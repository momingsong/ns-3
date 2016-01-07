#include "network_adapter.h"

#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

namespace ns3 {
namespace pec {

void SendPacket(Ptr<Socket> socket, Ptr<Packet> packet) {
  socket->Send(packet);
}

const uint16_t kPecUdpPort = 7320;
const InetSocketAddress kPecUdpAddress = InetSocketAddress(
                                             Ipv4Address("255.255.255.255"),
                                             kPecUdpPort);

NetworkAdapter::NetworkAdapter(Application &context,
                               MessageReceiverInterface &receiver)
  : context_(context), receiver_(receiver) { }

NetworkAdapter::~NetworkAdapter() {
  if (receive_socket_ != 0) {
    receive_socket_->SetRecvCallback(MakeNullCallback<void, Ptr<Socket> >());
  }
}

void NetworkAdapter::Init() {
  if (send_socket_ == 0) {
    TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
    send_socket_ = Socket::CreateSocket(context_.GetNode(), tid);
    send_socket_->Bind();
    send_socket_->SetAllowBroadcast(true);
    send_socket_->Connect(kPecUdpAddress);
  }
  send_socket_->SetRecvCallback(MakeNullCallback<void, Ptr<Socket> >());

  if (receive_socket_ == 0) {
    TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
    receive_socket_ = Socket::CreateSocket(context_.GetNode(), tid);
    InetSocketAddress local = InetSocketAddress(Ipv4Address::GetAny(), 
                                                kPecUdpPort);
    receive_socket_->Bind(local);
  }
  receive_socket_->SetRecvCallback(MakeCallback(&NetworkAdapter::Receive, this));
}

void NetworkAdapter::SendInterest(::pec::Interest &interest, double max_backoff) {
  Send(interest, max_backoff);
}

void NetworkAdapter::SendData(::pec::Data &data, double max_backoff) {
  /*
  NS_LOG_UNCOND("SendData: " << data.nonce());
  for (int i = 0; i < data.metadata().size(); ++i) {
    NS_LOG_UNCOND(data.metadata()[i]);
  }
  */
  Send(data, max_backoff);
}

void NetworkAdapter::Send(::pec::Block &message, double max_backoff) {
  uint32_t len = message.GetWireLength();
  uint8_t *buf = new uint8_t[len];
  message.GetWire(buf, len);
  Ptr<Packet> packet = Create<Packet>(buf, len);
  if (max_backoff <= 0.0) {
    send_socket_->Send(packet);
  } else {
    double backoff = (rand() % 10000) / 10000.0 * max_backoff;
    Simulator::Schedule(Seconds(backoff), &SendPacket, send_socket_, packet);
    delete [] buf;
  }
}

void NetworkAdapter::Receive(Ptr<Socket> socket) {
  Ptr<Packet> packet;
  Address sender;
  while ((packet = socket->RecvFrom(sender))) {
    uint32_t len = packet->GetSize();
    uint8_t *buf = new uint8_t[len];
    packet->CopyData(buf, len);
    Ipv4Address from_ip = InetSocketAddress::ConvertFrom(sender).GetIpv4();
    ::pec::TlvType type = ::pec::PeekType(buf, len);
    if (type == ::pec::kTlvInterest) {
      ::pec::Interest interest;
      interest.DecodeFromBuffer(buf, len);
      receiver_.ReceiveInterest(interest, from_ip);
    } else if (type == ::pec::kTlvData) {
      ::pec::Data data;
      data.DecodeFromBuffer(buf, len);
      receiver_.ReceiveData(data, from_ip);
    }
    delete [] buf;
  }
}

} // namespace pec
} // namespace ns3