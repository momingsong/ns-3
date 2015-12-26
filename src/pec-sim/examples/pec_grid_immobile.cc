#include <iostream>
#include <sstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/callback.h"

#include "ns3/pec-sim-module.h"

#include <sys/time.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("PecSimple");

void DataDiscovery(Ptr<ns3::pec::App> consumer) {
  consumer->DataDiscovery();
}

int main(int argc, char *argv[]) {

  LogComponentEnable("pec.App", LOG_LEVEL_ALL);

  //
  // Parameters
  //

  // PEC default parameters

  std::string trace_path = "trace";

  int node_num = 100;
  int grid_width = 10;
  double grid_interval = 100.0;
  
  int consumer_index = 44;

  int data_amount = 10000;
  int redundancy = 1;

  bool enable_redundancy_detection = true;

  bool enable_collision_avoidance = true;
  int max_metadata_per_message = -1; // set to -1 if no limit
  double max_backoff = 0.0; 
  
  bool enable_multi_round = true;
  double mr_round_finish_threshold = 0.0;
  double mr_discovery_finish_threshold = 0.0;
  double mr_slot_size = 1.0;

  // Parse commandline parameters
  CommandLine cmd;
  cmd.AddValue("tracePath", "Path of the output trace file", trace_path);
  cmd.AddValue("nodeNum", "Number of nodes", node_num);
  cmd.AddValue("gridWidth", "Number of nodes in a row", grid_width);
  cmd.AddValue("gridInterval", "Distance between adjacent nodes in the grid",
                grid_interval);
  cmd.AddValue("consumerIdx", "Index of the consumer node", consumer_index);
  cmd.AddValue("dataAmount",
               "How many different pieces of data exist in the network",
               data_amount);
  cmd.AddValue("redundancy",
               "How many copies of a same piece of data exist in the network",
               redundancy);
  cmd.AddValue("enableRedundancyDetection",
               "Enable redundancy detection functions",
               enable_redundancy_detection);
  cmd.AddValue("enableCollisionAvoidance",
               "Enable collision avoidance functions",
               enable_collision_avoidance);
  cmd.AddValue("maxMetadataPerMessage",
               "Max number of metadata entries carried by a Data message",
               max_metadata_per_message);
  cmd.AddValue("maxBackoff", "Max time of the application level random backoff",
               max_backoff);
  cmd.AddValue("enableMultiRound",
               "Enable multi-round data discovery mode",
               enable_multi_round);
  cmd.AddValue("multiRoundRoundFinishThreshold",
               "Multi Round: TH_R. Threshold on \"data messages received in current slot\" / \"data messages received in current round\" to terminate a round",
               mr_round_finish_threshold);
  cmd.AddValue("multiRoundDiscoveryFinishThreshold",
               "Multi Round: TH_D. Threshold on \"new metadata received in current round\" / \"new metadata received in current data discovery\" to terminate data discovery",
               mr_discovery_finish_threshold);
  cmd.AddValue("multiRoundSlotSize",
               "Multi Round: Size of a time slot in multi-round data discovery.",
               mr_slot_size);
  cmd.Parse (argc, argv);

  // Log parameters
  std::stringstream stream;
  stream << "# tracePath=" << trace_path << std::endl
         << "# nodeNum=" << node_num << std::endl
         << "# consumerIdx=" << consumer_index << std::endl
         << "# gridWidth=" << grid_width << std::endl
         << "# gridInterval=" << grid_interval << std::endl
         << "# consumerIdx=" << consumer_index << std::endl
         << "# dataAmount=" << data_amount << std::endl
         << "# redundancy=" << redundancy << std::endl
         << "# enableRedundancyDetection=" << enable_redundancy_detection << std::endl
         << "# enableCollisionAvoidance=" << enable_collision_avoidance << std::endl
         << "# maxMetadataPerMessage=" << max_metadata_per_message << std::endl
         << "# maxBackoff=" << max_backoff << std::endl
         << "# enableMultiRound=" << enable_multi_round << std::endl
         << "# multiRoundRoundFinishThreshold=" << mr_round_finish_threshold << std::endl
         << "# multiRoundDiscoveryFinishThreshold=" << mr_discovery_finish_threshold << std::endl
         << "# multiRoundSlotSize=" << mr_slot_size << std::endl;
  std::string parameters = stream.str();
  std::cout << parameters;

  //
  // Nodes
  //

  NodeContainer nodes;
  nodes.Create(node_num);

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
                      StringValue ("DsssRate1Mbps"));

  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi;
  wifi.SetStandard(WIFI_PHY_STANDARD_80211b);

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

  // Add a non-QoS upper mac, and disable rate control
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default();
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                               "DataMode", StringValue("DsssRate1Mbps"),
                               "ControlMode", StringValue("DsssRate1Mbps"));
  // Set it to adhoc mode
  wifiMac.SetType("ns3::AdhocWifiMac");
  NetDeviceContainer devices = wifi.Install(wifiPhy, wifiMac, nodes);

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
                                "DeltaX", DoubleValue (grid_interval),
                                "DeltaY", DoubleValue (grid_interval),
                                "GridWidth", UintegerValue (grid_width),
                                "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(nodes);

  //
  // PEC Application
  //

  Config::SetDefault("ns3::pec::App::EnableRedundancyDetection",
                     BooleanValue(enable_redundancy_detection));
  Config::SetDefault("ns3::pec::App::EnableCollisionAvoidance",
                     BooleanValue(enable_collision_avoidance));
  Config::SetDefault("ns3::pec::App::CAMaxMetadataPerMessage",
                     IntegerValue(max_metadata_per_message));
  Config::SetDefault("ns3::pec::App::CAMaxBackoff",
                     DoubleValue(max_backoff));
  Config::SetDefault("ns3::pec::App::EnableMultiRound",
                     BooleanValue(enable_multi_round));
  Config::SetDefault("ns3::pec::App::MRRoundFinishThreshold",
                     DoubleValue(mr_round_finish_threshold));
  Config::SetDefault("ns3::pec::App::MRDiscoveryFinishThreshold",
                     DoubleValue(mr_discovery_finish_threshold));
  Config::SetDefault("ns3::pec::App::MRSlotSize",
                     DoubleValue(mr_slot_size));

  ns3::pec::AppHelper helper;
  helper.set_data_amount(data_amount);
  helper.set_redundancy(redundancy);
 
  ApplicationContainer apps = helper.Install(nodes);
  ns3::pec::PecTracer tracer(trace_path, parameters, ip_container);
  tracer.TraceApps(apps);

  //
  // Simulation
  //

  time_t begin_time;
  time(&begin_time);

  Ptr<ns3::pec::App> consumer = DynamicCast<ns3::pec::App>(apps.Get(consumer_index));
  Simulator::ScheduleWithContext(consumer_index, Seconds(1.0), &DataDiscovery, consumer);
  Simulator::Run();
  Simulator::Destroy();

  tracer.Output();

  time_t end_time;
  time(&end_time);

  double diff = difftime(end_time, begin_time);

  std::cout << "Simulation finished in " << diff << " seconds." << std::endl;

  return 0;
}
