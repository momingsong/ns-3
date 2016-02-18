#ifndef PEC_TRACER_H_
#define PEC_TRACER_H_

#include <set>
#include <string>
#include <fstream>
#include <vector>

#include "ns3/application-container.h"
#include "ns3/ipv4-interface-container.h"

#include "data_discovery_tracer.h"

namespace ns3 {
namespace pec {

class PecTracer {
 public:
  PecTracer(std::string prefix, std::string comment,
            Ipv4InterfaceContainer &ip_container);
  ~PecTracer();

  void TraceApps(ApplicationContainer apps);
  void Output();
  void SetConsumerLog(int consumer_index, bool consumer_log);

 private:
  Ptr<Node> GetNodeFromIp(Ipv4Address address);
  // Callbacks
  void StartDataDiscovery(Ptr<App> app);
  void SendInterest(Ptr<App> app, Ipv4Address from_ip, int nonce, int hop_nonce, uint32_t size, const bloom_filter & filter);
  void SendData(Ptr<App> app, Ipv4Address from_ip, std::set<Ipv4Address> to_ips, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata);
  void ReceiveInterest(Ptr<App> app, Ipv4Address from_ip, int nonce, int hop_nonce, uint32_t size, const bloom_filter & filter);
  void WillReceiveData(Ptr<App> app, Ipv4Address from_ip, std::set<Ipv4Address> to_ips, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata);
  void DidReceiveData(Ptr<App> app, Ipv4Address from_ip, std::set<Ipv4Address> to_ips, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata);
  void SendAck(Ptr<App> app, int nonce, int hop_nonce, Ipv4Address from_ip, uint32_t size);
  void ReceiveAck(Ptr<App> app, int nonce, int hop_nonce, Ipv4Address from_ip);
  void Retransmit(Ptr<App> app, int nonce, int hop_nonce, uint32_t size);
  void SendECData(Ptr<App> app, int nonce, int hop_nonce, int idx, uint32_t len);
  void ReceiveECData(Ptr<App> app, int nonce, int hop_nonce, int idx, uint32_t len);

  std::string prefix_;
  std::string comment_;
  Ipv4InterfaceContainer ip_container_;

  std::ofstream list_;

  std::vector<DataDiscoveryTracer *> tracers_;

  ApplicationContainer apps_;

  // overhead
  int num_;
  int message_num_;
  int interest_num_;
  int data_num_;
  int ack_num_;
  uint32_t message_size_;
  uint32_t interest_size_;
  uint32_t data_size_;
  uint32_t ack_size_;
  int consumer_index_;//Only for log use
  bool consumer_log_;

};

inline void PecTracer::SetConsumerLog(int consumer_index, bool consumer_log)
{
  consumer_index = consumer_index_;
  consumer_log = consumer_log_;
}

} // namespace pec
} // namespace ns3

#endif // PEC_TRACER_H_