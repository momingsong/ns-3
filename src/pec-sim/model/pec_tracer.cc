#include "pec_tracer.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/log.h"

namespace ns3 {
namespace pec {

PecTracer::PecTracer(std::string prefix, std::string comment,
                     Ipv4InterfaceContainer &ip_container) {
  prefix_ = prefix;
  comment_ = comment;
  ip_container_ = ip_container;

  message_num_ = 0;
  interest_num_ = 0;
  data_num_ = 0;
  message_size_ = 0;
  interest_size_ = 0;
  data_size_ = 0;
  ack_num_=0;
  ack_size_=0;

  //for log use
  consumer_index_ = 0;
  consumer_log_ = false;

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
  apps_=apps;
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
      "WillReceiveData", 
      MakeCallback(&PecTracer::WillReceiveData, this));
    app->TraceConnectWithoutContext(
      "DidReceiveData", 
      MakeCallback(&PecTracer::DidReceiveData, this));
    app->TraceConnectWithoutContext(
      "SendAck", 
      MakeCallback(&PecTracer::SendAck, this));
    app->TraceConnectWithoutContext(
      "ReceiveAck", 
      MakeCallback(&PecTracer::ReceiveAck, this));
    app->TraceConnectWithoutContext(
      "Retransmit", 
      MakeCallback(&PecTracer::Retransmit, this));
    app->TraceConnectWithoutContext(
      "SendECData", 
      MakeCallback(&PecTracer::SendECData, this));
    app->TraceConnectWithoutContext(
      "ReceiveECData", 
      MakeCallback(&PecTracer::ReceiveECData, this));
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
    << ((double)data_size_ / data_num_) <<" "
    <<message_num_<<" "
    <<message_size_<<" "
    <<interest_num_<<" "
    <<interest_size_<<" "
    <<data_num_<<" "
    <<data_size_<<" "
    <<ack_num_<<" "
    <<ack_size_<<" "

    << std::endl;
  output.close();
}

Ptr<Node> PecTracer::GetNodeFromIp(Ipv4Address address) {
  Ipv4InterfaceContainer::Iterator iter = ip_container_.Begin();
  while (iter != ip_container_.End()) {
    if (iter->first->GetInterfaceForAddress(address) != -1) {
      return iter->first->GetObject<Node>();
    }
    ++iter;
  }
  return NULL;
}

void PecTracer::StartDataDiscovery(Ptr<App> app) {
  NS_LOG_UNCOND("SDD: " << app->GetNode()->GetId());
  double start_time = Simulator::Now().GetSeconds();
  int index = tracers_.size();
  char indexChar[10];
  sprintf(indexChar, "%d", index);
  DataDiscoveryTracer *tracer = new DataDiscoveryTracer(prefix_ + "_" + indexChar + ".data", comment_, start_time, prefix_, apps_);
  tracer->TraceApp(app);
  tracers_.push_back(tracer);
  list_ << index << " " << app->GetNode()->GetId() << " " << start_time << std::endl;
}

void PecTracer::SendInterest(Ptr<App> app, Ipv4Address from_ip, int nonce, int hop_nonce, uint32_t size, const bloom_filter & filter) {

  std::stringstream fip_ss;
  from_ip.Print(fip_ss);
  NS_LOG_UNCOND("SI: " << app->GetNode()->GetId() << " "
                       << fip_ss.str() << " "
                       << nonce << " " 
                       << hop_nonce << " "
                       << size << " " 
                       << filter.size());

  std::ofstream output;
  output.open(std::string(prefix_ + "_SI.data").c_str(),std::fstream::out | std::fstream::app);
  output<<"SI: " << app->GetNode()->GetId()<< " " <<Simulator::Now().GetSeconds()<< " Package:" << nonce<<"  Hop:"<<hop_nonce<<" Fip:"<<fip_ss.str()<<" "<<size<<" "<< filter.size() <<std::endl;

  message_size_ += size;
  interest_size_ += size;
  ++message_num_;
  ++interest_num_;
}

void PecTracer::SendData(Ptr<App> app, Ipv4Address from_ip, std::set<Ipv4Address> to_ips, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata) {

  std::stringstream fip_ss;
  from_ip.Print(fip_ss);
  std::stringstream tip_ss;
  std::set<Ipv4Address>::iterator iter = to_ips.begin();
  iter->Print(tip_ss);
  ++iter;
  while (iter != to_ips.end()) {
    tip_ss << "|";
    iter->Print(tip_ss);
    ++iter;
  }
  NS_LOG_UNCOND("SD: " << app->GetNode()->GetId() << " "
                       << fip_ss.str() << " "
                       << tip_ss.str() << " "
                       << nonce << " " 
                       << hop_nonce << " "
                       << size << " " 
                       << metadata.size());

std::ofstream output;
  output.open(std::string(prefix_ + "_SD.data").c_str(),std::fstream::out | std::fstream::app);
output<<"SD: " << app->GetNode()->GetId()<< " " <<Simulator::Now().GetSeconds()<< " Package:" << nonce<<"  Hop:"<<hop_nonce<<" Fip:"<<fip_ss.str()<<" Toip"<<tip_ss.str()<<" "<<size<<" "<< metadata.size()<<std::endl;


  message_size_ += size;
  data_size_ += size;
  ++message_num_;
  ++data_num_;
}

void PecTracer::ReceiveInterest(Ptr<App> app, Ipv4Address from_ip, int nonce, int hop_nonce, uint32_t size, const bloom_filter & filter) {

  std::stringstream fip_ss;
  from_ip.Print(fip_ss);
  NS_LOG_UNCOND("RI: " << app->GetNode()->GetId() << " " 
                       << fip_ss.str() << " "
                       << nonce << " " 
                       << hop_nonce << " "
                       << size << " "
                       << filter.size());
                std::ofstream output;
                output.open(std::string(prefix_ + "_RI.data").c_str(),std::fstream::out | std::fstream::app);
                output<<"RI: " << app->GetNode()->GetId()<< " " <<Simulator::Now().GetSeconds()<< " Package:" << nonce<<"  Hop:"<<hop_nonce<<" Fip:"<<fip_ss.str()<<" "<<size<<" "<< filter.size() <<std::endl;
}

void PecTracer::WillReceiveData(Ptr<App> app, Ipv4Address from_ip, std::set<Ipv4Address> to_ips, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata) {

  std::stringstream fip_ss;
  from_ip.Print(fip_ss);
  std::stringstream tip_ss;
  std::set<Ipv4Address>::iterator iter = to_ips.begin();
  iter->Print(tip_ss);
  ++iter;
  while (iter != to_ips.end()) {
    tip_ss << "|";
    iter->Print(tip_ss);
    ++iter;
  }
  NS_LOG_UNCOND("WRD: " << app->GetNode()->GetId() << " " 
                        << fip_ss.str() << " "
                        << tip_ss.str() << " "
                        << nonce << " " 
                        << hop_nonce << " "
                        << size << " "
                        << metadata.size());


std::ofstream output;
  output.open(std::string(prefix_ + "_WRD.data").c_str(),std::fstream::out | std::fstream::app);
output<<"WRD: " << app->GetNode()->GetId()<< " " <<Simulator::Now().GetSeconds()<< " Package:" << nonce<<"  Hop:"<<hop_nonce<<" Fip:"<<fip_ss.str()<<" Toip"<<tip_ss.str()<<" "<<size<<" "<< metadata.size()<<std::endl;
}

void PecTracer::DidReceiveData(Ptr<App> app, Ipv4Address from_ip, std::set<Ipv4Address> to_ips, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata) {

  std::stringstream fip_ss;
  from_ip.Print(fip_ss);
  std::stringstream tip_ss;
  std::set<Ipv4Address>::iterator iter = to_ips.begin();
  iter->Print(tip_ss);
  ++iter;
  while (iter != to_ips.end()) {
    std::stringstream ss;
    ss << "|";
    iter->Print(ss);
    ++iter;
  }
  NS_LOG_UNCOND("DRD: " << app->GetNode()->GetId() << " " 
                        << fip_ss.str() << " "
                        << tip_ss.str() << " "
                        << nonce << " " 
                        << hop_nonce << " "
                        << size << " "
                        << metadata.size());

std::ofstream output;
  output.open(std::string(prefix_ + "_DRD.data").c_str(),std::fstream::out | std::fstream::app);
output<<"DRD: " << app->GetNode()->GetId()<< " " <<Simulator::Now().GetSeconds()<< " Package:" << nonce<<"  Hop:"<<hop_nonce<<" Fip:"<<fip_ss.str()<<" Toip"<<tip_ss.str()<<" "<<size<<" "<< metadata.size()<<std::endl;

}

void PecTracer::SendAck(Ptr<App> app, int nonce, int hop_nonce, Ipv4Address from_ip, uint32_t size) {
  std::stringstream fip_ss;
  from_ip.Print(fip_ss);
  NS_LOG_UNCOND("SA: " << app->GetNode()->GetId() << " "
                       << nonce << " "
                       << hop_nonce << " "
                       << fip_ss.str());
  std::ofstream output;
  output.open(std::string(prefix_ + "_SK.data").c_str(),std::fstream::out | std::fstream::app);
output<<"SK: " << app->GetNode()->GetId()<< " " <<Simulator::Now().GetSeconds()<< " Package:" << nonce<<"  Hop:"<<hop_nonce<<" Fip:"<<fip_ss.str()<<std::endl;
  message_size_ += size;
  ack_size_ += size;
  ++message_num_;
  ++ack_num_;
}

void PecTracer::ReceiveAck(Ptr<App> app, int nonce, int hop_nonce, Ipv4Address from_ip) {
  std::stringstream fip_ss;
  from_ip.Print(fip_ss);
  NS_LOG_UNCOND("RA: " << app->GetNode()->GetId() << " "
                       << nonce << " "
                       << hop_nonce << " "
                       << fip_ss.str());
  std::ofstream output;
  output.open(std::string(prefix_ + "_RK.data").c_str(),std::fstream::out | std::fstream::app);
output<<"RK: " << app->GetNode()->GetId()<< " " <<Simulator::Now().GetSeconds()<< " Package:" << nonce<<"  Hop:"<<hop_nonce<<" Fip:"<<fip_ss.str()<<std::endl;
}

void PecTracer::Retransmit(Ptr<App> app, int nonce, int hop_nonce, uint32_t size) {
  NS_LOG_UNCOND("RT: " << app->GetNode()->GetId() << " "
                       << nonce << " "
                       << hop_nonce);
  std::ofstream output;
  output.open(std::string(prefix_ + "_T.data").c_str(),std::fstream::out | std::fstream::app);
  output<<"T: " << app->GetNode()->GetId()<< " " <<Simulator::Now().GetSeconds()<< " Package:" << nonce<<"  Hop:"<<hop_nonce<<std::endl;
  message_size_ += size;
  data_size_ += size;
  ++message_num_;
  ++data_num_;
}

void PecTracer::SendECData(Ptr<App> app, int nonce, int hop_nonce, int idx, uint32_t len) {
  NS_LOG_UNCOND("SED: " << app->GetNode()->GetId() << " "
                        << nonce << " "
                        << hop_nonce << " "
                        << idx << " "
                        << len);
std::ofstream output;
  output.open(std::string(prefix_ + "_SED.data").c_str(),std::fstream::out | std::fstream::app);
output<<"SED: " << app->GetNode()->GetId()<< " " <<Simulator::Now().GetSeconds()<< " Package:" << nonce<<"  Hop:"<<hop_nonce<<" Block "<<len<<std::endl;
}


void PecTracer::ReceiveECData(Ptr<App> app, int nonce, int hop_nonce, int idx, uint32_t len) {
  NS_LOG_UNCOND("RED: " << app->GetNode()->GetId() << " "
                        << nonce << " "
                        << hop_nonce << " "
                        << idx << " "
                        << len);
                        
std::ofstream output;
  output.open(std::string(prefix_ + "_RED.data").c_str(),std::fstream::out | std::fstream::app);
output<<"RED: " << app->GetNode()->GetId()<< " " <<Simulator::Now().GetSeconds()<< " Package:" << nonce<<"  Hop:"<<hop_nonce<<" Block "<<len<<std::endl;

}

} // namespace pec
} // namespace ns3
