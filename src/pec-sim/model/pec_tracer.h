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

 private:
  Ptr<Node> GetNodeFromIp(Ipv4Address address);
  // Callbacks
  void StartDataDiscovery(Ptr<App> app);
  void SendInterest(Ptr<App> app, Ipv4Address from_ip, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata);
  void SendData(Ptr<App> app, Ipv4Address from_ip, std::set<Ipv4Address> to_ips, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata);
  void ReceiveInterest(Ptr<App> app, Ipv4Address from_ip, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata);
  void WillReceiveData(Ptr<App> app, Ipv4Address from_ip, std::set<Ipv4Address> to_ips, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata);
  void DidReceiveData(Ptr<App> app, Ipv4Address from_ip, std::set<Ipv4Address> to_ips, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata);

  std::string prefix_;
  std::string comment_;
  Ipv4InterfaceContainer ip_container_;

  std::ofstream list_;

  std::vector<DataDiscoveryTracer *> tracers_;

  // overhead
  int num_;
  int message_num_;
  int interest_num_;
  int data_num_;
  uint32_t message_size_;
  uint32_t interest_size_;
  uint32_t data_size_;

};

} // namespace pec
} // namespace ns3

#endif // PEC_TRACER_H_