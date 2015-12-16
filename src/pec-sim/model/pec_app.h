#ifndef PEC_APP_H_
#define PEC_APP_H_

#include <vector>
#include <set>
#include <queue>

#include "ns3/application.h"
#include "ns3/traced-callback.h"

#include "interest.h"
#include "data.h"
#include "message_receiver_interface.h"
#include "network_adapter.h"
#include "pending_interest_table.h"

namespace ns3 {
namespace pec {

class App : public Application, public ::pec::MessageReceiverInterface {
 public:
  static TypeId GetTypeId();

  App();
  ~App();

  virtual void ReceiveInterest(::pec::Interest interest);
  virtual void ReceiveData(::pec::Data data);

  void AddMetadata(int metadata) { local_metadata_.insert(metadata); }
  bool HasMetadata(int metadata);

  void DataDiscovery();

  const std::set<int> &local_metadata() { return local_metadata_; }

 protected:
  virtual void DoInitialize();
  virtual void DoDispose();

 private:
  virtual void StartApplication();
  virtual void StopApplication();

  void RequestMetadata();
  void NextRound(bool is_first_round);
  void NextSlot();

  void SendInterest(::pec::Interest interest);
  void SendData(::pec::Data data, std::set<int> receivers);

  ::pec::PendingInterestTable pit_;
  std::set<int> local_metadata_;
  std::set<int> data_nonces_;

  NetworkAdapter network_adapter_;

  //
  // Parameters
  //

  // redundancy detection parameters
  bool enable_redundancy_detection_;

  // collision avoidance parameters
  bool enable_collision_avoidance_;
  int max_metadata_per_message_;
  double max_backoff_;

  // multi-round parameters
  bool enable_multi_round_;
  double mr_round_finish_threshold_; // TH_R
  double mr_discovery_finish_threshold_; // TH_D
  double mr_slot_size_;

  // multi-round state information
  int metadata_count_discovery_;
  int metadata_count_round_;
  int data_message_count_round_;
  int data_message_count_slot_;

  // 
  // Tracing
  //

  TracedCallback<Ptr<App> > start_data_discovery_callback_;

  // parameteres for the following 4 callbacks:
  // AppPointer, MessageNonce, MessageSize, MetadataSetReference
  TracedCallback<Ptr<App>, int, uint32_t, const std::set<int> &> send_interest_message_callback_;
  TracedCallback<Ptr<App>, int, uint32_t, const std::set<int> &> send_data_message_callback_;
  TracedCallback<Ptr<App>, int, uint32_t, const std::set<int> &> receive_interest_message_callback_;
  TracedCallback<Ptr<App>, int, uint32_t, const std::set<int> &> receive_data_message_callback_;
};

} // namespace pec
} // namespace ns3

#endif // PEC_APP_H_