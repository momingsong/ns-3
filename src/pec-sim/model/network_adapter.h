#ifndef PEC_NETWORK_ADAPTER_H_
#define PEC_NETWORK_ADAPTER_H_

#include <map>
#include <set>

#include "ns3/application.h"
#include "ns3/socket.h"
#include "ns3/ipv4.h"

#include "interest.h"
#include "data.h"
#include "ack.h"
#include "message_receiver_interface.h"
#include "ec_data.h"
#include "erasure_code.h"
#include "ec_ack.h"

namespace ns3 {
namespace pec {

class NetworkAdapter {
 public:
  NetworkAdapter(Application &context, 
                 MessageReceiverInterface &receiver);
  ~NetworkAdapter();

  static void ConfigRetransmit(bool enable_retransmit, double timeout, int retry) {
    enable_retransmit_ = enable_retransmit;
    timeout_ = timeout;
    retry_ = retry;
  }

  static void ConfigErasureCoding(bool enable_erasure_code, int k, int m) {
    enable_erasure_code_ = enable_erasure_code;
    ::pec::ErasureCode::ConfigErasureCode(k, m);
  }

  void Init();
  void SendInterest(::pec::Interest interest, double max_backoff);
  void SendData(::pec::Data data, double max_backoff);
  void SendAck(::pec::Ack ack);

 private:
  void Send(::pec::Block &message, bool retransmit, int retry);
  void ECSend(::pec::Data &data, bool retransmit, int retry);
  void Retransmit(::pec::Data& message, int retry);
  void ECRetransmit(::pec::Data &ddata, int retry);
  void Receive(Ptr<Socket> socket);
  void ReceiveAck(::pec::Ack ack);
  void ReceiveECData(::pec::ECData ec_data, Ipv4Address from_ip);
  void ReceiveECAck(::pec::ECAck ec_ack);

  static bool enable_retransmit_;
  static double timeout_;
  static int retry_;

  static bool enable_erasure_code_;

  Application &context_;
  MessageReceiverInterface &receiver_;
  Ptr<Socket> send_socket_;
  Ptr<Socket> receive_socket_;

  std::map<int, std::set<uint32_t> > waiting_ack_; // hop_nonce -> receivers
  std::map<int, std::map<uint32_t, std::vector<bool> > > waiting_ec_ack_; // hop_nonce -> receivers -> blocks

  std::map<int, bool> ec_decoded_;  // hop_nonce -> is_decoded
  std::map<int, std::vector< ::pec::ECData> > ec_received_;  // hop_nonce -> received_ec_data_list
};

} // namespace pec
} // namespace ns3

#endif // PEC_NETWORK_ADAPTER_H_
