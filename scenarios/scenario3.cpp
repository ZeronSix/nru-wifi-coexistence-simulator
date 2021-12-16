#include "simulator.hpp"
#include "channel.hpp"
#include "config.hpp"
#include "ue_node.hpp"
#include "enb_node.hpp"
#include "wifi_sta_node.hpp"
#include "wifi_ap_node.hpp"
#include "on_off_switcher.hpp"

int main(int argc, char *argv[]) {
  cfg::ParseCommandLineArgs(argc, argv);
  Simulator::Configure();

  if (cfg::g_wifiTOnMs < 2.5) {
    cfg::g_wifiFrameSize = cfg::WIFI_DEFAULT_FRAME_SIZE * cfg::g_wifiTOnMs / 2.5;
  }

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

  const Time tOn = cfg::g_wifiTOnMs * 1000;
  const Time tOff = tOn / cfg::g_wifiLoad - tOn;

  std::vector<Ptr<WifiApNode>> aps;
  std::vector<Ptr<WifiStaNode>> stas;
  std::vector<Ptr<OnOffSwitcher>> switchers;
  for (size_t i = 0; i < cfg::g_numCarriers; ++i) {
    aps.push_back(MakeNode<WifiApNode>(carriers[i]));
    stas.push_back(MakeNode<WifiStaNode>(carriers[i], aps.back()));
    auto staPtr = stas.back();
    switchers.push_back(std::make_shared<OnOffSwitcher>(
        tOn,
        tOff,
        [staPtr](bool value) {
          staPtr->SwitchOn(value);
        },
        0
    ));
  }

  Simulator::Run();

  double totalThroughput = 0;
  for (auto ue : ues) {
    totalThroughput += 1.0 * ue->GetRxBytes() / cfg::g_simTime;
  }

  std::cout << cfg::g_run << '\t'
            << cfg::g_simTime / 1000000 << '\t'
            << cfg::g_numCarriers << '\t'
            << cfg::g_numEnbs << '\t'
            << LbtTypeToStr(cfg::g_lbtType) << '\t'
            << cfg::g_txLock << '\t'
            << cfg::g_wifiLoad << '\t'
            << cfg::g_wifiTOnMs << '\t'
	    << PrimaryTypeToStr(cfg::g_primaryCarrierType) << '\t'
            << totalThroughput << '\t'
            << std::endl;

  return 0;
}
