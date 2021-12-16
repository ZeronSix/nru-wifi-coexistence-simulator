#include "simulator.hpp"
#include "channel.hpp"
#include "config.hpp"
#include "wifi_ap_node.hpp"
#include "wifi_sta_node.hpp"
#include "on_off_switcher.hpp"
#include <vector>

int main(int argc, char *argv[]) {
  cfg::ParseCommandLineArgs(argc, argv);
  Simulator::Configure();

  Ptr<Channel> channel = std::make_shared<Channel>();
  Ptr<WifiApNode> ap = MakeNode<WifiApNode>(channel);

  std::vector<Ptr<WifiStaNode>> stas;
  for (size_t i = 0; i < cfg::g_numWifiStas; ++i) {
    stas.emplace_back(MakeNode<WifiStaNode>(channel, ap));
  }

  Simulator::Run();

  double totalThroughput = 8.0 * ap->GetRxBytes() / cfg::g_simTime;
  std::cout << cfg::g_run << '\t'
            << cfg::g_simTime / 1000000 << '\t'
            << cfg::g_numWifiStas << '\t'
            << totalThroughput << '\t'
            << std::endl;

  return 0;
}
