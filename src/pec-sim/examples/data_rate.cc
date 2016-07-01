#include <iostream>
#include <sstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/callback.h"
#include "ns3/applications-module.h"

#include "ns3/pec-sim-module.h"

#include <sys/time.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("PecDataRate");

int main(int argc, char *argv[]) {

  LogComponentEnable("pec.App", LOG_LEVEL_ALL);

  //
  // Nodes
  //

  NodeContainer nodes;
  int n = 5;
  int r = 2;
  nodes.Create(n);

  //
  // Wifi Phy
  //

  // disable fragmentation for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold",
                      StringValue ("2200"));
  // turn off RTS/CTS for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold",
                      StringValue ("2200"));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",
                       StringValue ("HtMcs0"));

  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi;
  wifi.SetStandard(WIFI_PHY_STANDARD_80211n_2_4GHZ);

  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
  // This is one parameter that matters when using FixedRssLossModel
  // set it to zero; otherwise, gain will be added
  wifiPhy.Set("RxGain", DoubleValue(0));
  // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
  wifiPhy.SetPcapDataLinkType(YansWifiPhyHelper::DLT_IEEE802_11_RADIO);

  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
  // The below FixedRssLossModel will cause the rss to be fixed regardless
  // of the distance between the two stations, and the transmit power
  wifiChannel.AddPropagationLoss("ns3::LogDistancePropagationLossModel");
  wifiPhy.SetChannel(wifiChannel.Create());
  // Set guard interval
  wifiPhy.Set("ShortGuardEnabled", BooleanValue(1));

  // Add a non-QoS upper mac, and disable rate control
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default();
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                               "DataMode", StringValue("HtMcs7"),
                               "ControlMode", StringValue("HtMcs7"));
  // Set it to adhoc mode
  wifiMac.SetType("ns3::AdhocWifiMac");
  NetDeviceContainer devices = wifi.Install(wifiPhy, wifiMac, nodes);

  // Set channel width
  Config::Set("NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue(20));

  //
  // IP
  //

  InternetStackHelper internet;
  internet.Install (nodes);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer ip_container = ipv4.Assign (devices);

  //
  // Mobility
  //

  MobilityHelper mobility;
  mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                "MinX", DoubleValue (0.0),
                                "MinY", DoubleValue (0.0),
                                "DeltaX", DoubleValue (0.5),
                                "DeltaY", DoubleValue (0.5),
                                "GridWidth", UintegerValue (2),
                                "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(nodes);

  //
  // PEC Application
  //

  ObjectFactory factory_;
  factory_.SetTypeId(ns3::pec::PecUdpServer::GetTypeId());
  Ptr<ns3::pec::PecUdpServer> server = factory_.Create<ns3::pec::PecUdpServer>();
  nodes.Get(r)->AddApplication(server);
  ApplicationContainer servers;
  servers.Add(server);
  servers.Start(Seconds (1.0));
  servers.Stop(Seconds (10000.0));

  factory_.SetTypeId(ns3::pec::PecUdpClient::GetTypeId());
  ApplicationContainer clients;
  for (int i = 0; i < n; ++i) {
    if (i == r)
      continue;
    Ptr<ns3::pec::PecUdpClient> client = factory_.Create<ns3::pec::PecUdpClient>();
    nodes.Get(i)->AddApplication(client);
    clients.Add(client);
  }
  clients.Start(Seconds (2.0));
  clients.Stop(Seconds (10000.0));

  //
  // Simulation
  //

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  return 0;
}
