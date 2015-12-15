#ifndef PEC_APP_HELPER_H_
#define PEC_APP_HELPER_H_

#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"

#include "ns3/pec_app.h"

namespace ns3 {
namespace pec {

class AppHelper {
 public:
  AppHelper();
  ApplicationContainer Install(NodeContainer nodes);

  void set_data_amount(int data_amount) { data_amount_ = data_amount; }
  int data_amount() { return data_amount_; }

  void set_redundancy(int redundancy) { redundancy_ = redundancy; }
  int redundancy() { return redundancy_; }

  /*
  void set_metadata_num(int metadata_num) { metadata_num_ = metadata_num; }
  int metadata_num() { return metadata_num_; }

  void set_delay(double delay) { delay_ = delay; }
  double delay() { return delay_; }
  */

 private:
  ObjectFactory factory_;

  int data_amount_;
  int redundancy_;

  // int max_metadata_per_message_;
  // double max_backoff_;
};

} // namespace pec
} // namespace ns3

#endif // PEC_APP_HELPER_H_