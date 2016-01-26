#include "network_adapter.h"

#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

namespace ns3 {
namespace pec {

const uint16_t kPecUdpPort = 7320;
const InetSocketAddress kPecUdpAddress = InetSocketAddress(
                                             Ipv4Address("255.255.255.255"),
                                             kPecUdpPort);

bool NetworkAdapter::enable_retransmit_ = true;
double NetworkAdapter::timeout_ = 0.5;
int NetworkAdapter::retry_ = 3;
bool NetworkAdapter::enable_erasure_code_ = true;

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

void NetworkAdapter::SendInterest(::pec::Interest interest, double max_backoff) {
  if (max_backoff <= 0.0) {
    Send(interest, false, 0);
  } else {
    double backoff = (rand() % 10000) / 10000.0 * max_backoff;
    Simulator::Schedule(Seconds(backoff), &NetworkAdapter::Send, this, interest, false, 0);
  }
}

void NetworkAdapter::SendData(::pec::Data data, double max_backoff) {
  waiting_ack_.insert(std::pair<int, std::set<uint32_t> >(data.hop_nonce(), data.receivers()));
  if (max_backoff <= 0.0) {
    if (enable_erasure_code_) {
      ECSend(data, true, 0);
    } else {
      Send(data, true, 0);
    }
  } else {
    double backoff = (rand() % 10000) / 10000.0 * max_backoff;
    if (enable_erasure_code_) {
      Simulator::Schedule(Seconds(backoff), &NetworkAdapter::ECSend, this, data, true, 0);
    } else {
      Simulator::Schedule(Seconds(backoff), &NetworkAdapter::Send, this, data, true, 0);
    }
  }
}

void NetworkAdapter::SendAck(::pec::Ack ack) {
  if (enable_retransmit_) {
    receiver_.SendAckCallback(ack.nonce(), ack.hop_nonce(), ack.from());
    Send(ack, false, 0);
  }
}

void NetworkAdapter::Send(::pec::Block &message, bool retransmit, int retry) {
  uint32_t len = message.GetWireLength();
  uint8_t *buf = new uint8_t[len];
  message.GetWire(buf, len);
  Ptr<Packet> packet = Create<Packet>(buf, len);
  send_socket_->Send(packet);
  if (enable_retransmit_ && retransmit) {
    Simulator::Schedule(Seconds(timeout_), &NetworkAdapter::Retransmit, this, (::pec::Data&)message, retry + 1);
  }
  delete [] buf;
}

void NetworkAdapter::ECSend(::pec::Data &data, bool retransmit, int retry) {
  std::vector< ::pec::ECData> ec_data = ::pec::ECData::EncodeData(data);
  for (unsigned int i = 0; i < ec_data.size(); ++i) {
    // tracing
    receiver_.SendECDataCallback(
      ec_data[i].nonce(),
      ec_data[i].hop_nonce(),
      ec_data[i].k(),
      ec_data[i].m(),
      ec_data[i].idx(),
      ec_data[i].GetWireLength()
    );
    uint32_t len = ec_data[i].GetWireLength();
    uint8_t *buf = new uint8_t[len];
    ec_data[i].GetWire(buf, len);
    Ptr<Packet> packet = Create<Packet>(buf, len);
    send_socket_->Send(packet);
    delete [] buf;
  }
  if (enable_retransmit_ && retransmit) {
    Simulator::Schedule(Seconds(timeout_), &NetworkAdapter::ECRetransmit, this, data, retry + 1);
  }
}

void NetworkAdapter::Retransmit(::pec::Data &message, int retry) {
  if (retry >= retry_)
    return;
  if (!waiting_ack_.find(message.hop_nonce())->second.empty()) {
    message.set_receivers(waiting_ack_.find(message.hop_nonce())->second);
    receiver_.RetransmitCallback(message.nonce(), message.hop_nonce());
    Send(message, true, retry);
  } else {
    waiting_ack_.erase(message.hop_nonce());
  }
}

void NetworkAdapter::ECRetransmit(::pec::Data &data, int retry) {
  if (retry >= retry_)
    return;
  if (!waiting_ack_.find(data.hop_nonce())->second.empty()) {
    data.set_receivers(waiting_ack_.find(data.hop_nonce())->second);
    receiver_.RetransmitCallback(data.nonce(), data.hop_nonce());
    ECSend(data, true, retry);
  } else {
    waiting_ack_.erase(data.hop_nonce());
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
    } else if (type == ::pec::kTlvAck) {
      ::pec::Ack ack;
      ack.DecodeFromBuffer(buf, len);
      ReceiveAck(ack);
    } else if (type == ::pec::kTlvECData) {
      ::pec::ECData ec_data;
      ec_data.DecodeFromBuffer(buf, len);
      ReceiveECData(ec_data, from_ip);
    }
    delete [] buf;
  }
}

void NetworkAdapter::ReceiveAck(::pec::Ack ack) {
  receiver_.ReceiveAckCallback(ack.nonce(), ack.hop_nonce(), ack.from());
  if (waiting_ack_.find(ack.hop_nonce()) != waiting_ack_.end()) {
    waiting_ack_.find(ack.hop_nonce())->second.erase(ack.from());
  }
}

void NetworkAdapter::ReceiveECData(::pec::ECData ec_data, Ipv4Address from_ip) {
  // tracing
  receiver_.ReceiveECDataCallback(
    ec_data.nonce(),
    ec_data.hop_nonce(),
    ec_data.k(),
    ec_data.m(),
    ec_data.idx(),
    ec_data.GetWireLength()
  );
  int hop_nonce = ec_data.hop_nonce();
  if (ec_received_.find(hop_nonce) == ec_received_.end()) {
    std::vector< ::pec::ECData> received;
    ec_received_.insert(std::pair<int, std::vector< ::pec::ECData> >(hop_nonce, received));
    ec_decoded_.insert(std::pair<int, bool>(hop_nonce, false));
  }

  if (ec_decoded_.find(hop_nonce)->second) {
    return;
  }

  std::vector< ::pec::ECData> &received = ec_received_.find(hop_nonce)->second;
  std::vector< ::pec::ECData>::iterator iter = received.begin();
  while (iter != received.end()) {
    if (ec_data.idx() == iter->idx()) {
      return;
    }
    ++iter;
  }

  received.push_back(ec_data);

  if (received.size() >= (unsigned int)ec_data.k()) {
    ::pec::Data data = ::pec::ECData::DecodeData(received);
    receiver_.ReceiveData(data, from_ip);
    ec_decoded_.find(hop_nonce)->second = true;
  }
}

} // namespace pec
} // namespace ns3