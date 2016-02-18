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

NS_LOG_COMPONENT_DEFINE("PecMultiConsumer");

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
  
  int consumer_num = 5;
  int mc_min_interval = 100;
  int mc_max_interval = 150;

  int metadata_entry_size = 30;
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
  int mr_window_size = 5;

  int bf_size_max = 60000;
  int bf_size_min = 10000;
  double bf_fpp = 0.01;

  bool enable_retransmit = true;
  double rt_timeout = 0.5;
  int rt_retry = 3;

  bool enable_erasure_code = true;
  int ec_k = 10;
  int ec_m = 2;

  bool enable_consumer_log = true;
  int seednum = 1;


  // Parse commandline parameters
  CommandLine cmd;
  cmd.AddValue("tracePath", "Path of the output trace file", trace_path);
  cmd.AddValue("nodeNum", "Number of nodes", node_num);
  cmd.AddValue("gridWidth", "Number of nodes in a row", grid_width);
  cmd.AddValue("gridInterval", "Distance between adjacent nodes in the grid",
                grid_interval);
  cmd.AddValue("consumerNum", "Number of consumers", consumer_num);
  cmd.AddValue("multiConsumerMinInterval",
               "Multi Consumer: mimimum interval in seconds between two consumer request.",
               mc_min_interval);
  cmd.AddValue("multiConsumerMaxInterval",
               "Multi Consumer: maximum interval in seconds between two consumer request.",
               mc_max_interval);
  cmd.AddValue("metadataEntrySize", "Size of each metadata entry in bytes", metadata_entry_size);
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
  cmd.AddValue("multiRoundWindowSize",
               "Multi Round: Number of recent time slots to decide terminating a round in multi-round data discovery.",
               mr_window_size);
  cmd.AddValue("bloomFilterSizeMax",
               "Bloom Filter: Max size of bloom filter in bytes.",
               bf_size_max);
  cmd.AddValue("bloomFilterSizeMin",
               "Bloom Filter: Min size of bloom filter in bytes.",
               bf_size_min);
  cmd.AddValue("bloomFilterFalsePositive",
               "Bloom Filter: Expected probability of false positive.",
               bf_fpp);
  cmd.AddValue("enableRetransmit",
               "Enable retransmit.",
               enable_retransmit);
  cmd.AddValue("retransmitTimeout",
               "Retransmit: Time in seconds waiting for ACK before retransmiting.",
               rt_timeout);
  cmd.AddValue("retransmitRetry",
               "Retransmit: max time trying to send one message.",
               rt_retry);
  cmd.AddValue("enableErasureCoding",
               "Enable erasure coding.",
                enable_erasure_code);
  cmd.AddValue("erasureCodingK",
               "Erasure Coding: K.",
               ec_k);
  cmd.AddValue("erasureCodingM",
               "Erasure Coding: M.",
               ec_m);
  cmd.AddValue("enableConsumerLog",
                "Enable the detailed log for the consumer received package",
                enable_consumer_log);
  cmd.AddValue("seed",
                "Set the seed of the random numnber",
                seednum);
  cmd.Parse (argc, argv);

  // Log parameters
  std::stringstream stream;
  stream << "# tracePath=" << trace_path << std::endl
         << "# nodeNum=" << node_num << std::endl
         << "# gridWidth=" << grid_width << std::endl
         << "# gridInterval=" << grid_interval << std::endl
         << "# consumerNum=" << consumer_num << std::endl
         << "# multiConsumerMinInterval=" << mc_min_interval << std::endl
         << "# multiConsumerMaxInterval=" << mc_max_interval << std::endl
         << "# metadataEntrySize=" << metadata_entry_size << std::endl
         << "# dataAmount=" << data_amount << std::endl
         << "# redundancy=" << redundancy << std::endl
         << "# enableRedundancyDetection=" << enable_redundancy_detection << std::endl
         << "# enableCollisionAvoidance=" << enable_collision_avoidance << std::endl
         << "# maxMetadataPerMessage=" << max_metadata_per_message << std::endl
         << "# maxBackoff=" << max_backoff << std::endl
         << "# enableMultiRound=" << enable_multi_round << std::endl
         << "# multiRoundRoundFinishThreshold=" << mr_round_finish_threshold << std::endl
         << "# multiRoundDiscoveryFinishThreshold=" << mr_discovery_finish_threshold << std::endl
         << "# multiRoundSlotSize=" << mr_slot_size << std::endl
         << "# multiRoundWindowSize=" << mr_window_size << std::endl
         << "# bloomFilterSizeMax=" << bf_size_max << std::endl
         << "# bloomFilterSizeMin=" << bf_size_min << std::endl
         << "# bloomFilterFalsePositive=" << bf_fpp << std::endl
         << "# enableRetransmit=" << enable_retransmit << std::endl
         << "# retransmitTimeout=" << rt_timeout << std::endl
         << "# retransmitRetry=" << rt_retry << std::endl
         << "# enableErasureCoding=" << enable_erasure_code << std::endl
         << "# erasureCodingK=" << ec_k << std::endl
         << "# erasureCodingM=" << ec_m << std::endl;
  std::string parameters = stream.str();
  std::cout << parameters;

  //
  // Set random number
  //
  srand((size_t)seednum);

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
  Config::SetDefault("ns3::pec::App::MRWindowSize",
                     IntegerValue(mr_window_size));
  ::pec::Interest::ConfigBloomFilter(bf_size_max, bf_size_min, bf_fpp);
  ::pec::Data::ConfigMetadata(metadata_entry_size);
  ::ns3::pec::NetworkAdapter::ConfigRetransmit(enable_retransmit, rt_timeout, rt_retry);
  ::ns3::pec::NetworkAdapter::ConfigErasureCoding(enable_erasure_code, ec_k, ec_m);

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

  double t = 1.0;
  std::vector<int> consumer_list;
  for (int i = 0; i < consumer_num; ++i) {
    
    int consumer_index = rand() % node_num;
    while (consumer_index/10>6 || consumer_index/10<2 || consumer_index%10>6 || consumer_index%10<2)
          consumer_index = rand() % node_num;
    //For 100 node num
    for(size_t jj = 0; jj<consumer_list.size(); jj++)
    {
      if (consumer_index == consumer_list[jj])
      {
        consumer_index = rand() % node_num;
        while (consumer_index/10>6 || consumer_index/10<2 || consumer_index%10>6 || consumer_index%10<2)
          consumer_index = rand() % node_num;
        jj=0;
      }
    }
    consumer_list.push_back(consumer_index);

    Ptr<ns3::pec::App> consumer = DynamicCast<ns3::pec::App>(apps.Get(consumer_index));
    if (mc_max_interval - mc_min_interval > 0) {
      t += (rand() % 10000) / 10000.0 * (mc_max_interval - mc_min_interval) + mc_min_interval;
    } else {
      t += mc_min_interval;
    }
    Simulator::ScheduleWithContext(consumer_index, Seconds(t), &ns3::pec::App::DataDiscovery, consumer);
  }
  Simulator::Run();
  Simulator::Destroy();
  tracer.Output();

  time_t end_time;
  time(&end_time);

  double diff = difftime(end_time, begin_time);

  std::cout << "Simulation finished in " << diff << " seconds." << std::endl;

  return 0;
}
