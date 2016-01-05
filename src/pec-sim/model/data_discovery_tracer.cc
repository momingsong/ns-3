#include "data_discovery_tracer.h"

#include "ns3/simulator.h"

namespace ns3 {
namespace pec {

DataDiscoveryTracer::DataDiscoveryTracer(
  std::string file, std::string comment, double start_time) {
  file_ = file;
  comment_ = comment;
  start_time_ = start_time;

  output_.open(file.c_str());
  output_ << comment_;
}

DataDiscoveryTracer::~DataDiscoveryTracer() {
  if (output_.is_open()) {
    output_.close();
  }
}

void DataDiscoveryTracer::TraceApp(Ptr<App> app) {
  app->TraceConnectWithoutContext(
    "WillReceiveData", MakeCallback(&DataDiscoveryTracer::WillReceiveData, this));
  app->TraceConnectWithoutContext(
    "DidReceiveData", MakeCallback(&DataDiscoveryTracer::DidReceiveData, this));
  metadata_num_ = app->local_metadata().size();
  output_ << 0 << " " << metadata_num_ << " 0 0 I" << std::endl;
}


void DataDiscoveryTracer::WillReceiveData(Ptr<App> app, Ipv4Address from_ip, std::set<Ipv4Address> to_ips, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata)
{
  output_ << Simulator::Now().GetSeconds() - start_time_ << " "
          << app->local_metadata().size() <<" " <<nonce<<" "<<hop_nonce<<" w"<<std::endl;
}
void DataDiscoveryTracer::DidReceiveData(Ptr<App> app, Ipv4Address from_ip, std::set<Ipv4Address> to_ips, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata)
{
  output_ << Simulator::Now().GetSeconds() - start_time_ << " "
          << app->local_metadata().size() <<" " <<nonce<<" "<<hop_nonce<<" d"<<std::endl;
}
} // namespace pec
} // namespace ns3
