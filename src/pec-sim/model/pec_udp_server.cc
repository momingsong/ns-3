#include "pec_udp_server.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

namespace ns3 {
namespace pec {

uint16_t PecUdpServer::port_ = 9876;

TypeId PecUdpServer::GetTypeId() {
	static TypeId tid = TypeId("ns3::pec::PecUdpServer")
		.SetParent<Application>()
		.AddConstructor<PecUdpServer>();
	return tid;
}

PecUdpServer::PecUdpServer() {

}

PecUdpServer::~PecUdpServer() {

}

void PecUdpServer::DoDispose() {
	Application::DoDispose();
}

void PecUdpServer::StartApplication() {
	if (socket_ == 0) {
		TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
		socket_ = Socket::CreateSocket(GetNode(), tid);
		InetSocketAddress local = InetSocketAddress(Ipv4Address::GetAny(), port_);
		socket_->Bind(local);
	}
	socket_->SetRecvCallback(MakeCallback(&PecUdpServer::Receive, this));
}

void PecUdpServer::StopApplication() {
	if (socket_ != 0) {
		socket_->SetRecvCallback(MakeNullCallback<void, Ptr<Socket> >());
	}
	for (std::map<uint32_t, int>::iterator iter = counters_.begin(); iter != counters_.end(); ++iter) {
		std::cout << "total: " << iter->first << " " << iter->second << std::endl;
	}
}

void PecUdpServer::Receive(Ptr<Socket> socket) {
	Ptr<Packet> packet;
	Address from;
	while ((packet = socket->RecvFrom(from))) {
		if (packet->GetSize() > 0) {
			Ipv4Address ip = InetSocketAddress::ConvertFrom(from).GetIpv4();
			uint32_t key = ip.Get();
			counters_[key]++;
			uint32_t len = packet->GetSize();
    		uint8_t *buf = new uint8_t[len];
    		packet->CopyData(buf, len);
    		int *p = (int *)buf;
    		std::cout << Simulator::Now().GetSeconds() << " " << key << " " << *p << std::endl;
		}
	}
}


} // namespace pec
} // namespace ns3