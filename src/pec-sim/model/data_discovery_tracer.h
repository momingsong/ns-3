#ifndef PEC_DATA_DISCOVERY_TRACER_H_
#define PEC_DATA_DISCOVERY_TRACER_H_

#include <string>
#include <fstream>
#include <set>

#include "ns3/ptr.h"
#include "ns3/application-container.h"

#include "pec_app.h"

namespace ns3 {
namespace pec {

class DataDiscoveryTracer {
 public:
  DataDiscoveryTracer(
    std::string file, std::string comment, double start_time, std::string traceprefix, ApplicationContainer apps);
  ~DataDiscoveryTracer();

  void TraceApp(Ptr<App> app);

 private:
  void WillReceiveData(Ptr<App> app, Ipv4Address from_ip, std::set<Ipv4Address> to_ips, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata);
  void DidReceiveData(Ptr<App> app, Ipv4Address from_ip, std::set<Ipv4Address> to_ips, int nonce, int hop_nonce, uint32_t size, const std::set<int> & metadata);
  void SendFirstInterest(Ptr<App> app, Ipv4Address from_ip, int nonce, int hop_nonce, uint32_t size, const bloom_filter & filter);

  ApplicationContainer apps_;

  std::string file_;
  std::string comment_;
  std::string prefix_;
  double start_time_;

  std::ofstream output_;

  int metadata_num_;

};

} // namespace pec
} // namespace ns3

#endif // PEC_DATA_DISCOVERY_TRACER_H_