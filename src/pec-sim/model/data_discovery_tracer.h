#ifndef PEC_DATA_DISCOVERY_TRACER_H_
#define PEC_DATA_DISCOVERY_TRACER_H_

#include <string>
#include <fstream>
#include <set>

#include "ns3/ptr.h"

#include "pec_app.h"

namespace ns3 {
namespace pec {

class DataDiscoveryTracer {
 public:
  DataDiscoveryTracer(
    std::string file, std::string comment, double start_time);
  ~DataDiscoveryTracer();

  void TraceApp(Ptr<App> app);

 private:
  void ReceiveData(Ptr<App> app, int nonce, uint32_t size, const std::set<int> & metadata);

  std::string file_;
  std::string comment_;
  double start_time_;

  std::ofstream output_;

  int metadata_num_;

};

} // namespace pec
} // namespace ns3

#endif // PEC_DATA_DISCOVERY_TRACER_H_