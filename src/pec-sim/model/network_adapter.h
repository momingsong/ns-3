#ifndef PEC_NETWORK_ADAPTER_H_
#define PEC_NETWORK_ADAPTER_H_

#include <map>
#include <set>

#include "ns3/application.h"
#include "ns3/socket.h"

#include "interest.h"
#include "data.h"
#include "ack.h"
#include "message_receiver_interface.h"

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

  void Init();
  void SendInterest(::pec::Interest interest, double max_backoff);
  void SendData(::pec::Data data, double max_backoff);
  void SendAck(::pec::Ack ack);

 private:
  void Send(::pec::Block &message, bool retransmit, int retry);
  void Retransmit(::pec::Block &message, int retry);
  void Receive(Ptr<Socket> socket);
  void ReceiveAck(::pec::Ack ack);

  static bool enable_retransmit_;
  static double timeout_;
  static int retry_;

  Application &context_;
  MessageReceiverInterface &receiver_;
  Ptr<Socket> send_socket_;
  Ptr<Socket> receive_socket_;

  std::map<int, std::set<uint32_t> > waiting_ack_; // hop_nonce -> receivers
};

} // namespace pec
} // namespace ns3

#endif // PEC_NETWORK_ADAPTER_H_
