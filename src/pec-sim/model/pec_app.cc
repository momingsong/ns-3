#include "pec_app.h"

#include "ns3/log.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/integer.h"
#include "ns3/double.h"
#include "ns3/boolean.h"
#include "ns3/simulator.h"

namespace ns3 {
namespace pec {

NS_LOG_COMPONENT_DEFINE("pec.App");
NS_OBJECT_ENSURE_REGISTERED(App);

TypeId App::GetTypeId() {
  static TypeId tid = 
    TypeId("ns3::pec::App")
      .SetParent<Application>()
      .AddConstructor<App>()
      // Redundancy Detection
      .AddAttribute("EnableRedundancyDetection", 
                    "Enable redundancy detection functions",
                    BooleanValue(true),
                    MakeBooleanAccessor(&App::enable_redundancy_detection_),
                    MakeBooleanChecker())
      // Collision Avoidance
      .AddAttribute("EnableCollisionAvoidance", 
                    "Enable collision avoidance functions",
                    BooleanValue(true),
                    MakeBooleanAccessor(&App::enable_collision_avoidance_),
                    MakeBooleanChecker())
      .AddAttribute("CAMaxMetadataPerMessage",
                    "Collision Avoidance: Max number of metadata entries carried by a Data message.",
                    IntegerValue(-1),
                    MakeIntegerAccessor(&App::max_metadata_per_message_),
                    MakeIntegerChecker<int>())
      .AddAttribute("CAMaxBackoff",
                    "Collision Avoidance: Max time of the application level random backoff",
                    DoubleValue(0.0),
                    MakeDoubleAccessor(&App::max_backoff_),
                    MakeDoubleChecker<double>())
      // Multi Round
      .AddAttribute("EnableMultiRound",
                    "Enable multi-round data discovery mode",
                    BooleanValue(true),
                    MakeBooleanAccessor(&App::enable_multi_round_),
                    MakeBooleanChecker())
      .AddAttribute("MRRoundFinishThreshold",
                    "Multi Round: TH_R. Threshold on \"data messages received in current slot\" / \"data messages received in current round\" to terminate a round",
                    DoubleValue(0.0),
                    MakeDoubleAccessor(&App::mr_round_finish_threshold_),
                    MakeDoubleChecker<double>())
      .AddAttribute("MRDiscoveryFinishThreshold",
                    "Multi Round: TH_D. Threshold on \"new metadata received in current round\" / \"new metadata received in current data discovery\" to terminate data discovery",
                    DoubleValue(0.0),
                    MakeDoubleAccessor(&App::mr_discovery_finish_threshold_),
                    MakeDoubleChecker<double>())
      .AddAttribute("MRSlotSize",
                    "Multi Round: Size of a time slot in multi-round data discovery.",
                    DoubleValue(1.0),
                    MakeDoubleAccessor(&App::mr_slot_size_),
                    MakeDoubleChecker<double>())
      .AddAttribute("MRWindowSize",
                    "Multi Round: Number of recent time slots to decide terminating a round in multi-round data discovery.",
                    IntegerValue(5),
                    MakeIntegerAccessor(&App::mr_window_size_),
                    MakeIntegerChecker<int>())

      // Tracing
      .AddTraceSource("StartDataDiscovery",
                      "Start data discovery",
                      MakeTraceSourceAccessor(&App::start_data_discovery_callback_),
                      "ns3::pec::App::RequestMetadata")
      .AddTraceSource("SendInterest",
                      "Send interest message to the network",
                      MakeTraceSourceAccessor(&App::send_interest_message_callback_),
                      "ns3::pec::App::SendInterest")
      .AddTraceSource("SendData",
                      "Send data message to the network",
                      MakeTraceSourceAccessor(&App::send_data_message_callback_),
                      "ns3::pec::App::SendData")
      .AddTraceSource("ReceiveInterest",
                      "Receive ineterest message from the network",
                      MakeTraceSourceAccessor(&App::receive_interest_message_callback_),
                      "ns3::pec::App::ReceiveInterest")
      .AddTraceSource("WillReceiveData",
                      "Will receive datas message from the network",
                      MakeTraceSourceAccessor(&App::will_receive_data_message_callback_),
                      "ns3::pec::App::ReceiveData")
      .AddTraceSource("DidReceiveData",
                      "Did receive datas message from the network",
                      MakeTraceSourceAccessor(&App::did_receive_data_message_callback_),
                      "ns3::pec::App::ReceiveData");
  return tid;
}

App::App() : network_adapter_(*this, *this) {
  metadata_count_discovery_ = 0;
  metadata_count_round_ = 0;
  data_message_count_round_ = 0;
  data_message_count_slot_ = 0;
}

App::~App() { }

void App::ReceiveInterest(::pec::Interest interest, Ipv4Address from_ip) {
  // tracing
  receive_interest_message_callback_(
    this,
    from_ip,
    interest.nonce(),
    interest.hop_nonce(),
    enable_redundancy_detection_ ? ::pec::Interest::kSizeWithBloomFilter 
                                 : ::pec::Interest::kSizeWithoutBloomFilter,
    interest.metadata()
  );

  // loop detection
  if (pit_.Exist(interest.nonce()))
    return;
  pit_.AddInterest(interest);

  // redundancy detection
  std::set<int> irredundant_metadata;
  if (enable_redundancy_detection_) {
    std::set<int>::iterator iter = local_metadata_.begin();
    while (iter != local_metadata_.end()) {
      if (!interest.HasMetadata(*iter))
        irredundant_metadata.insert(*iter);
      ++iter;
    }
    // modify interest message and the cached copy
    interest.AddMetadata(irredundant_metadata);
    pit_.UpdateInterest(interest);
  }

  // send interest message
  SendInterest(interest);

  // generate data messages
  std::vector< ::pec::Data> datas;
  if (enable_redundancy_detection_) {
    if (enable_collision_avoidance_) {
      datas = ::pec::Data::WrapMetadata(irredundant_metadata, max_metadata_per_message_);
    } else {
      datas = ::pec::Data::WrapMetadata(irredundant_metadata, -1);
    }
  } else {
    if (enable_collision_avoidance_) {
      datas = ::pec::Data::WrapMetadata(local_metadata_, max_metadata_per_message_);
    } else {
      datas = ::pec::Data::WrapMetadata(local_metadata_, -1);
    }
  }

  // send data messages
  std::set<uint32_t> recs;
  recs.insert(from_ip.Get());
  for (size_t i = 0; i < datas.size(); ++i) {
    SendData(datas[i], recs);
  }
}

void App::ReceiveData(::pec::Data data, Ipv4Address from_ip) {

  // tracing
  std::set<Ipv4Address> to_ips;
  std::set<uint32_t>::iterator iter = data.receivers().begin();
  while (iter != data.receivers().end()) {
    to_ips.insert(Ipv4Address(*iter));
    ++iter;
  }
  will_receive_data_message_callback_(
    this,
    from_ip,
    to_ips,
    data.nonce(),
    data.hop_nonce(),
    data.size(),
    data.metadata()
  );

  // loop detection
  if (data_nonces_.find(data.nonce()) == data_nonces_.end()) {
    data_nonces_.insert(data.nonce());

    // update local metadata and multi-round state information
    int prev_size = local_metadata_.size();
    std::set<int>::iterator iter = data.metadata().begin();
    while (iter != data.metadata().end()) {
      if (!HasMetadata(*iter)) {
        AddMetadata(*iter);
      }
      ++iter;
    }

    metadata_count_discovery_ += local_metadata_.size() - prev_size;
    metadata_count_round_ += local_metadata_.size() - prev_size;
    ++data_message_count_round_;
    ++data_message_count_slot_;

    if (data.receivers().find(GetIp().Get()) != data.receivers().end()) {
      // redundancy detection
      std::set<uint32_t> recs;
      if (enable_redundancy_detection_) {
        std::set<int> irredundant_metadata;
        irredundant_metadata = pit_.GetIrredundantMetadata(data.metadata(), recs);
        // modify data message
        data.set_metadata(irredundant_metadata);
        // modify cached interest messages
        pit_.AddMetadataToAll(irredundant_metadata);
      } else {
        recs = pit_.GetAllReceivers();
      }

      // send data message
      if (data.metadata().size() > 0 && recs.size() > 0) {
        SendData(data, recs);
      }
    }
  }

  // tracing
  did_receive_data_message_callback_(
    this,
    from_ip,
    to_ips,
    data.nonce(),
    data.hop_nonce(),
    data.size(),
    data.metadata()
  );
}

void App::DataDiscovery() {
  // NS_LOG_FUNCTION(this);
  
  start_data_discovery_callback_(this);

  if (enable_multi_round_) {
    NextRound(true);
  } else {
    RequestMetadata();
  }
}

void App::RequestMetadata() {
  // NS_LOG_FUNCTION(this);
  if (enable_redundancy_detection_) {
    ::pec::Interest interest(local_metadata_);
    SendInterest(interest);
  } else {
    ::pec::Interest interest;
    SendInterest(interest);
  }
}

void App::NextRound(bool is_first_round) {
  // NS_LOG_FUNCTION(this);
  if (is_first_round) {
    metadata_count_discovery_ = 0;
  }
  metadata_count_round_ = 0;
  data_message_count_round_ = 0;
  data_message_count_slot_ = 0;
  data_message_count_window_ = 0;
  while (!window_.empty()) {
    window_.pop();
  }
  RequestMetadata();
  Simulator::Schedule(Seconds(mr_slot_size_), &App::NextSlot, this);
}

void App::NextSlot() {
  // NS_LOG_FUNCTION(this);
  if (window_.size() == mr_window_size_) {
    data_message_count_window_ -= window_.front();
    window_.pop();
  }
  data_message_count_window_ += data_message_count_slot_;
  window_.push(data_message_count_slot_);

  if (window_.size() == mr_window_size_ &&
      (data_message_count_window_ == 0 ||
       ((double)data_message_count_window_) / ((double)data_message_count_round_)
       <= mr_round_finish_threshold_
      )
     ) { // round finished
    if ( metadata_count_round_ == 0 ||
      ((double)metadata_count_round_) / ((double)metadata_count_discovery_)
      <= mr_discovery_finish_threshold_) { // data discovery finished
      ;
    } else { // data discovery not finished
      NextRound(false);
    }
  } else { // round not finished
    data_message_count_slot_ = 0;
    Simulator::Schedule(Seconds(mr_slot_size_), &App::NextSlot, this);
  }
}

void App::SendInterest(::pec::Interest interest) {
  interest.set_sender(GetIp().Get());
  interest.set_hop_nonce(rand());

  // tracing
  send_interest_message_callback_(
    this,
    GetIp(),
    interest.nonce(), 
    interest.hop_nonce(),
    enable_redundancy_detection_ ? ::pec::Interest::kSizeWithBloomFilter 
                                 : ::pec::Interest::kSizeWithoutBloomFilter,
    interest.metadata()
  );

  if (enable_collision_avoidance_) {
    network_adapter_.SendInterest(interest, max_backoff_);
  } else {
    network_adapter_.SendInterest(interest, 0.0);
  }
}

void App::SendData(::pec::Data data, std::set<uint32_t> receivers) {
  data.set_receivers(receivers);
  data.set_hop_nonce(rand());
  
  // tracing
  std::set<Ipv4Address> to_ips;
  std::set<uint32_t>::iterator iter = data.receivers().begin();
  while (iter != data.receivers().end()) {
    to_ips.insert(Ipv4Address(*iter));
    ++iter;
  }
  send_data_message_callback_(
    this,
    GetIp(),
    to_ips,
    data.nonce(),
    data.hop_nonce(),
    data.size(),
    data.metadata()
  );

  if (enable_collision_avoidance_) {
    network_adapter_.SendData(data, max_backoff_);
  } else {
    network_adapter_.SendData(data, 0.0);
  }
}

bool App::HasMetadata(int metadata) {
  // NS_LOG_FUNCTION(this << metadata);
  return local_metadata_.find(metadata) != local_metadata_.end();
}

void App::DoInitialize() {
  // NS_LOG_FUNCTION(this);
  Application::DoInitialize();
}

void App::DoDispose() {
  // NS_LOG_FUNCTION(this);
  Application::DoDispose();
}

void App::StartApplication() {
  // NS_LOG_FUNCTION(this);
  network_adapter_.Init();
 }

void App::StopApplication() {
  // NS_LOG_FUNCTION(this);
}

} // namespace pec
} // namespace ns3
