#include "pec_app_helper.h"

#include <vector>

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/assert.h"

namespace ns3 {
namespace pec {

AppHelper::AppHelper() {
  factory_.SetTypeId(App::GetTypeId());
}

ApplicationContainer AppHelper::Install(NodeContainer nodes) {
  ApplicationContainer apps;

  for (NodeContainer::Iterator i = nodes.Begin(); i != nodes.End(); ++i) {
    Ptr<Node> node = *i;
    Ptr<App> app = factory_.Create<App>();

    // app->set_metadata_num(metadata_num_);
    // app->set_delay(delay_);
    
    node->AddApplication(app);
    apps.Add(app);
  }

  int node_num = apps.GetN();
  NS_ASSERT_MSG(redundancy_ < node_num, 
                "Bad parameters: number of nodes are less than redundancy.");
  for (int i = 0; i < data_amount_; ++i)
    for (int j = 0; j < redundancy_; ++j) {
      Ptr<App> app;
      while (true) {
        app = DynamicCast<App>(apps.Get(rand() % node_num));
        if (!app->HasMetadata(i)) {
          break;
        }
      }
      app->AddMetadata(i);
    }

  return apps;
}

} // namespace pec
} // namespace ns3
