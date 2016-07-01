#include "pec_udp_client.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

namespace ns3 {
namespace pec {

uint16_t PecUdpClient::port_ = 9876;

TypeId PecUdpClient::GetTypeId() {
	static TypeId tid = TypeId("ns3::pec::PecUdpClient")
		.SetParent<Application>()
		.AddConstructor<PecUdpClient>();
	return tid;
}

PecUdpClient::PecUdpClient() {
	count_ = 0;
}

PecUdpClient::~PecUdpClient() {

}

void PecUdpClient::DoDispose() {
	Application::DoDispose();
}

void PecUdpClient::StartApplication() {
	if (socket_ == 0) {
		TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
		socket_ = Socket::CreateSocket(GetNode(), tid);
		InetSocketAddress remote = InetSocketAddress(
                                             Ipv4Address("255.255.255.255"),
                                             port_);
		socket_->Bind();
		socket_->SetAllowBroadcast(true);
		socket_->Connect(remote);
	}
	socket_->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
  	send_event_ = Simulator::Schedule (Seconds ((rand() % 10000) / 10000.0 * 0.001), &PecUdpClient::Send, this);
}

void PecUdpClient::StopApplication() {
	Simulator::Cancel(send_event_);
}

void PecUdpClient::Send() {
	if (count_ < 10000) {
		uint32_t len = 1472;
		uint8_t *buf = new uint8_t[len];
  		int *p = (int *)buf;
  		*p = count_;
  		Ptr<Packet> packet = Create<Packet>(buf, len);
  		socket_->Send(packet);
		count_++;
		Simulator::Schedule(Seconds(0.0005), &PecUdpClient::Send, this);
	}
}

} // namespace pec
} // namespace ns3