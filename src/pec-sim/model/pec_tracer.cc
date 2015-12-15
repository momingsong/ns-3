#include "pec_tracer.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/log.h"

namespace ns3 {
namespace pec {

PecTracer::PecTracer(std::string prefix, std::string comment) {
  prefix_ = prefix;
  comment_ = comment;
  message_num_ = 0;
  interest_num_ = 0;
  data_num_ = 0;
  message_size_ = 0;
  interest_size_ = 0;
  data_size_ = 0;

  list_.open(prefix_+"_list.data");
}

PecTracer::~PecTracer() {
  if (list_.is_open()) {
    list_.close();
  }
  for (int i = 0; i < tracers_.size(); ++i)
    delete tracers_[i];
}

void PecTracer::TraceApps(ApplicationContainer apps) {
  num_ = apps.GetN();
  for (int i = 0; i < apps.GetN(); ++i) {
    Ptr<App> app = DynamicCast<App>(apps.Get(i));
    app->TraceConnectWithoutContext(
      "StartDataDiscovery", 
      MakeCallback(&PecTracer::StartDataDiscovery, this));
    app->TraceConnectWithoutContext(
      "SendInterest", 
      MakeCallback(&PecTracer::SendInterest, this));
    app->TraceConnectWithoutContext(
      "SendData", 
      MakeCallback(&PecTracer::SendData, this));
    app->TraceConnectWithoutContext(
      "ReceiveInterest", 
      MakeCallback(&PecTracer::ReceiveInterest, this));
    app->TraceConnectWithoutContext(
      "ReceiveData", 
      MakeCallback(&PecTracer::ReceiveData, this));
  }
}

void PecTracer::Output() {
  // output overhead
  std::ofstream output;
  output.open(prefix_ + "_overhead.data");
  output << comment_ 
    << ((double)message_num_) / num_ << " "
    << ((double)message_size_) / message_num_ << " "
    << ((double)interest_num_) / num_ << " "
    << ((double)interest_size_) / interest_num_ << " "
    << ((double)data_num_) / num_ << " " 
    << ((double)data_size_ / data_num_) << std::endl;
  output.close();
}

void PecTracer::StartDataDiscovery(Ptr<App> app) {
  double start_time = Simulator::Now().GetSeconds();
  int index = tracers_.size();
  DataDiscoveryTracer *tracer = new DataDiscoveryTracer(prefix_ + "_" + std::to_string(index) + ".data", comment_, start_time);
  tracer->TraceApp(app);
  tracers_.push_back(tracer);
  list_ << index << " " << app->GetNode()->GetId() << " " << start_time << std::endl;
}

void PecTracer::SendInterest(Ptr<App> app, int nonce, uint32_t size, const std::set<int> & metadata) {
  NS_LOG_UNCOND("SI: " << app->GetNode()->GetId() << " " 
                       << nonce << " " << size << " " << metadata.size());
  message_size_ += size;
  interest_size_ += size;
  ++message_num_;
  ++interest_num_;
}

void PecTracer::SendData(Ptr<App> app, int nonce, uint32_t size, const std::set<int> & metadata) {
  NS_LOG_UNCOND("SD: " << app->GetNode()->GetId() << " " 
                       << nonce << " " << size << " " << metadata.size());
  message_size_ += size;
  data_size_ += size;
  ++message_num_;
  ++data_num_;
}

void PecTracer::ReceiveInterest(Ptr<App> app, int nonce, uint32_t size, const std::set<int> & metadata) {
  NS_LOG_UNCOND("RI: " << app->GetNode()->GetId() << " " 
                       << nonce << " " << size << " " << metadata.size());
}

void PecTracer::ReceiveData(Ptr<App> app, int nonce, uint32_t size, const std::set<int> & metadata) {
  NS_LOG_UNCOND("RD: " << app->GetNode()->GetId() << " " 
                       << nonce << " " << size << " " << metadata.size());
}

} // namespace pec
} // namespace ns3