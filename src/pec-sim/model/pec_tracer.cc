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

  list_.open(std::string(prefix_+"_list.data").c_str());
}

PecTracer::~PecTracer() {
  if (list_.is_open()) {
    list_.close();
  }
  for (size_t i = 0; i < tracers_.size(); ++i)
    delete tracers_[i];
}

void PecTracer::TraceApps(ApplicationContainer apps) {
  num_ = apps.GetN();
  for (size_t i = 0; i < apps.GetN(); ++i) {
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
  output.open(std::string(prefix_ + "_overhead.data").c_str());
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
  char indexChar[10];
  sprintf(indexChar, "%d", index);
  DataDiscoveryTracer *tracer = new DataDiscoveryTracer(prefix_ + "_" + indexChar + ".data", comment_, start_time);
  tracer->TraceApp(app);
  tracers_.push_back(tracer);
  list_ << index << " " << app->GetNode()->GetId() << " " << start_time << std::endl;
}

void PecTracer::SendInterest(Ptr<App> app, int nonce, uint32_t size, const std::set<int> & metadata) {
<<<<<<< HEAD
std::ofstream output;
  output.open(std::string(prefix_ + "_SI.data").c_str(),std::fstream::out | std::fstream::app);
  NS_LOG_UNCOND("SI: " << app->GetNode()->GetId() << " " 
                       << nonce << " " << size << " " << metadata.size());
  output<<"SI: " << app->GetNode()->GetId() " " <<Simulator::Now().GetSeconds()<< " Package:" 
                       << nonce<<" "<< metadata.size() <<"\nContent: ";
  for(std::set<int>::iterator sit=metadata.begin();sit!=metadata.end();sit++)
	{
output<<*sit<<" ";
	}
output<<std::endl;
output<<"Local Package: ";
 for(std::set<int>::iterator lit=app->local_metadata().begin();lit!=app->local_metadata().end();lit++)
	{
output<<*lit<<" ";
	}
output<<std::endl;
  message_size_ += size;
  interest_size_ += size;
  ++message_num_;
  ++interest_num_;
}

void PecTracer::SendData(Ptr<App> app, int nonce, uint32_t size, const std::set<int> & metadata) {
  NS_LOG_UNCOND("SD: " << app->GetNode()->GetId() << " " 
                       << nonce << " " << size << " " << metadata.size()<<" ");
  message_size_ += size;
  data_size_ += size;
  ++message_num_;
  ++data_num_;
}

void PecTracer::ReceiveInterest(Ptr<App> app, int nonce, uint32_t size, const std::set<int> & metadata) {
  // NS_LOG_UNCOND("RI: " << app->GetNode()->GetId() << " " 
  //                      << nonce << " " << size << " " << metadata.size());
}

void PecTracer::ReceiveData(Ptr<App> app, int nonce, uint32_t size, const std::set<int> & metadata) {
  // NS_LOG_UNCOND("RD: " << app->GetNode()->GetId() << " " 
  //                      << nonce << " " << size << " " << metadata.size());
}

} // namespace pec
} // namespace ns3
