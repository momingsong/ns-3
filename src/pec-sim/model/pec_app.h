#ifndef PEC_APP_H_
#define PEC_APP_H_

#include <vector>
#include <set>
#include <queue>

#include "ns3/application.h"
#include "ns3/traced-callback.h"
#include "ns3/ipv4.h"

#include "interest.h"
#include "data.h"
#include "message_receiver_interface.h"
#include "network_adapter.h"
#include "pending_interest_table.h"

namespace ns3 {
namespace pec {

class App : public Application, public MessageReceiverInterface {
 public:
  static TypeId GetTypeId();

  App();
  ~App();

  virtual void ReceiveInterest(::pec::Interest interest, Ipv4Address from_ip);
  virtual void ReceiveData(::pec::Data data, Ipv4Address from_ip);
  virtual void SendAckCallback(int nonce, int hop_nonce, uint32_t from, uint32_t size);
  virtual void ReceiveAckCallback(int nonce, int hop_nonce, uint32_t from);
  virtual void RetransmitCallback(int nonce, int hop_nonce, uint32_t size);

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
  void SendData(::pec::Data data, std::set<uint32_t> receivers);

  Ipv4Address GetIp() {
    return GetNode()->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal();
  }

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
  int mr_window_size_;

  // multi-round state information
  int metadata_count_discovery_;
  int metadata_count_round_;
  int data_message_count_round_;
  int data_message_count_slot_;
  int data_message_count_window_;
  std::queue<int> window_;

  // 
  // Tracing
  //

  TracedCallback<Ptr<App> > start_data_discovery_callback_;

  TracedCallback<
    Ptr<App>,     // AppPointer
    Ipv4Address,  // FromIpAddress
    int,          // Nonce
    int,          // Hop Nonce
    uint32_t,     // MessageSize
    const bloom_filter & // BloomFilterReference
  > send_interest_message_callback_;

  TracedCallback<
    Ptr<App>,     // AppPointer
    Ipv4Address,  // FromIpAddress
    std::set<Ipv4Address>,  // ToIpAddresses
    int,          // Nonce
    int,          // HopNonce
    uint32_t,     // MessageSize
    const std::set<int> & // MetadataSetReference
  > send_data_message_callback_;

  TracedCallback<
    Ptr<App>,     // AppPointer
    Ipv4Address,  // FromIpAddress
    int,          // Nonce
    int,          // Hop Nonce
    uint32_t,     // MessageSize
    const bloom_filter & // BloomFilterReference
  > receive_interest_message_callback_;

  TracedCallback<
    Ptr<App>,     // AppPointer
    Ipv4Address,  // FromIpAddress
    std::set<Ipv4Address>,  // ToIpAddresses
    int,          // Nonce
    int,          // HopNonce
    uint32_t,     // MessageSize
    const std::set<int> & // MetadataSetReference
  > will_receive_data_message_callback_;

  TracedCallback<
    Ptr<App>,     // AppPointer
    Ipv4Address,  // FromIpAddress
    std::set<Ipv4Address>,  // ToIpAddresses
    int,          // Nonce
    int,          // HopNonce
    uint32_t,     // MessageSize
    const std::set<int> & // MetadataSetReference
  > did_receive_data_message_callback_;

  TracedCallback<
    Ptr<App>,     // AppPointer
    int,          // Nonce
    int,          // HopNonce
    Ipv4Address,   // FromIpAddress
    uint32_t      // ACK packet size
  > send_ack_callback_;

  TracedCallback<
    Ptr<App>,     // AppPointer
    int,          // Nonce
    int,          // HopNonce
    Ipv4Address   // FromIpAddress
  > receive_ack_callback_;

  TracedCallback<
    Ptr<App>,     // AppPointer
    int,          // Nonce
    int,          // HopNonce
    uint32_t      // Message size
  > retransmit_callback_;
};

} // namespace pec
} // namespace ns3

#endif // PEC_APP_H_
