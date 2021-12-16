#include "simulator.hpp"
#include "channel.hpp"
#include "config.hpp"
#include "ue_node.hpp"
#include "enb_node.hpp"
#include "wifi_sta_node.hpp"
#include "wifi_ap_node.hpp"

int main(int argc, char *argv[]) {
  cfg::ParseCommandLineArgs(argc, argv);
  Simulator::Configure();

  std::vector<Ptr<Channel>> carriers;
  for (size_t i = 0; i < cfg::g_numCarriers; ++i) {
    carriers.push_back(std::make_shared<Channel>());
  }
  std::vector<Ptr<UeNode>> ues;
  std::vector<Ptr<EnbNode>> enbs;
  for (size_t i = 0; i < cfg::g_numEnbs; ++i) {
    ues.push_back(MakeNode<UeNode>(carriers));
    enbs.push_back(MakeNode<EnbNode>(carriers, ues.back()));
  }

  std::vector<Ptr<WifiApNode>> aps;
  std::vector<Ptr<WifiStaNode>> stas;
  for (size_t i = 0; i < cfg::g_numCarriers; ++i) {
    for (size_t j = 0; j < cfg::g_numWifiStas; ++j) {
      aps.push_back(MakeNode<WifiApNode>(carriers[i]));
      stas.push_back(MakeNode<WifiStaNode>(carriers[i], aps.back()));
    }
  }

  Simulator::Run();

  double totalLaaThroughput = 0;
  for (const auto &ue : ues) {
    totalLaaThroughput += 1.0 * ue->GetRxBytes() / cfg::g_simTime;
  }

  double totalWifiThroughput = 0;
  for (const auto &ap : aps) {
    totalWifiThroughput += 8.0 * ap->GetRxBytes() / cfg::g_simTime;
  }

  std::cout << cfg::g_run << '\t'
            << cfg::g_simTime / 1000000 << '\t'
            << cfg::g_numCarriers << '\t'
            << cfg::g_numEnbs << '\t'
            << LbtTypeToStr(cfg::g_lbtType) << '\t'
            << cfg::g_numWifiStas << '\t'
            << cfg::PrimaryTypeToStr(cfg::g_primaryCarrierType) << '\t'
            << totalLaaThroughput << '\t'
            << totalWifiThroughput << '\t'
	    << cfg::g_txLock << '\t'
            << std::endl;

  return 0;
}
