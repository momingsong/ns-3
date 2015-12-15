#ifndef PEC_NETWORK_ADAPTER_H_
#define PEC_NETWORK_ADAPTER_H_

#include "ns3/application.h"
#include "ns3/socket.h"

#include "interest.h"
#include "data.h"
#include "message_receiver_interface.h"

namespace ns3 {
namespace pec {

class NetworkAdapter {
 public:
  NetworkAdapter(Application &context, 
                 ::pec::MessageReceiverInterface &receiver);
  ~NetworkAdapter();

  void Init();
  void SendInterest(::pec::Interest &interest, double max_backoff);
  void SendData(::pec::Data &data, double max_backoff);

 private:
  void Send(::pec::Block &message, double max_backoff);
  void Receive(Ptr<Socket> socket);

  Application &context_;
  ::pec::MessageReceiverInterface &receiver_;
  Ptr<Socket> send_socket_;
  Ptr<Socket> receive_socket_;

};

} // namespace pec
} // namespace ns3

#endif // PEC_NETWORK_ADAPTER_H_
