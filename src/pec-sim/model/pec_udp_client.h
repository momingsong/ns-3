#ifndef PEC_UDP_CLIENT_H_
#define PEC_UDP_CLIENT_H_

#include "ns3/application.h"
#include "ns3/socket.h"
#include "ns3/ipv4.h"
#include <map>

namespace ns3 {
namespace pec {

class PecUdpClient : public Application {
public:
  static TypeId GetTypeId();
  PecUdpClient();
  ~PecUdpClient();
protected:
  virtual void DoDispose();
private:
  virtual void StartApplication();
  virtual void StopApplication();

  void Send();

  static uint16_t port_;
  Ptr<Socket> socket_;
  EventId send_event_;

  int count_;
};

} // namespace pec
} // namespace ns3

#endif // PEC_UDP_CLIENT_H_
