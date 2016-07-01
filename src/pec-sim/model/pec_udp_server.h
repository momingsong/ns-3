#ifndef PEC_UDP_SERVER_H_
#define PEC_UDP_SERVER_H_

#include "ns3/application.h"
#include "ns3/socket.h"
#include "ns3/ipv4.h"
#include <map>

namespace ns3 {
namespace pec {

class PecUdpServer : public Application {
public:
  static TypeId GetTypeId();
  PecUdpServer();
  ~PecUdpServer();
protected:
  virtual void DoDispose();
private:
  virtual void StartApplication();
  virtual void StopApplication();

  void Receive(Ptr<Socket> socket);

  static uint16_t port_;
  Ptr<Socket> socket_;

  std::map<uint32_t, int> counters_;
};

} // namespace pec
} // namespace ns3

#endif // PEC_UDP_SERVER_H_
