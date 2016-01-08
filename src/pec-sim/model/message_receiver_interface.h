#ifndef PEC_MESSAGE_RECEIVER_INTERFACE_H_
#define PEC_MESSAGE_RECEIVER_INTERFACE_H_

#include "ns3/ipv4-address.h"

#include "interest.h"
#include "data.h"

namespace ns3 {
namespace pec {

class MessageReceiverInterface {
 public:
  virtual void ReceiveInterest(::pec::Interest interest, Ipv4Address from_ip) = 0;
  virtual void ReceiveData(::pec::Data data, Ipv4Address from_ip) = 0;

  virtual void SendAckCallback(int nonce, int hop_nonce, uint32_t from) = 0;
  virtual void ReceiveAckCallback(int nonce, int hop_nonce, uint32_t from) = 0;
  virtual void RetransmitCallback(int nonce, int hop_nonce) = 0;
};

} // namespace pec
} // namespace ns3

#endif // PEC_MESSAGE_RECEIVER_INTERFACE_H_