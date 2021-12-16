#include "simulator.hpp"
#include "channel.hpp"
#include "config.hpp"
#include "ue_node.hpp"
#include "enb_node.hpp"
#include "wifi_sta_node.hpp"
#include "wifi_ap_node.hpp"

int main(int argc, char* argv[])
{
    cfg::ParseCommandLineArgs(argc, argv);
    Simulator::Configure();

    std::vector<Channel> carriers(cfg::g_numCarriers);
    std::vector<UeNode> ues;
    std::vector<EnbNode> enbs;
    ues.reserve(cfg::g_numEnbs);
    enbs.reserve(cfg::g_numEnbs);
    for (size_t i = 0; i < cfg::g_numEnbs; ++i)
    {
        ues.emplace_back(UeNode(carriers));
        ues.back().Configure();
        enbs.emplace_back(EnbNode(carriers, ues.back()));
        enbs.back().Configure();
    }

    std::vector<WifiApNode> aps;
    std::vector<WifiStaNode> stas;
    aps.reserve(cfg::g_numWifiCarriers * cfg::g_numWifiStas);
    stas.reserve(cfg::g_numWifiCarriers * cfg::g_numWifiStas);
    for (size_t i = 0; i < cfg::g_numWifiCarriers; ++i)
    {
        for (size_t j = 0; j < cfg::g_numWifiStas; ++j)
        {
            aps.emplace_back(WifiApNode(carriers[i]));
            aps.back().Configure();
            stas.emplace_back(WifiStaNode(carriers[i], aps.back()));
            stas.back().Configure();
        }
    }

    Simulator::Run();

    double totalThroughput = 0;
    for (const auto& ue : ues)
    {
        totalThroughput += 1.0 * ue.GetRxBytes() / cfg::g_simTime;
        //std::cerr << 1.0 * ue.GetRxBytes() / cfg::g_simTime << std::endl;
    }
    // std::cerr << totalPackets / totalSuccessRatio << std::endl;
    double throughputPerCell = totalThroughput / cfg::g_numEnbs;
    double perCellVariance = 0;
    for (const auto& ue : ues)
    {
        auto x = (1.0 * ue.GetRxBytes() / cfg::g_simTime - throughputPerCell);
        perCellVariance += x * x / cfg::g_numEnbs;
    }

    double totalSuccessRatio = 0.0;
    for (const auto& enb : enbs)
    {
        totalSuccessRatio += enb.GetSuccessRatio();
    }
    totalSuccessRatio /= cfg::g_numEnbs;

    double totalChannelUsage = 0;
    for (const auto& carrier : carriers)
    {
        totalChannelUsage += cfg::g_simTime - carrier.GetTotalIdleTime();
        //std::cerr << 1.0 * (cfg::g_simTime - carrier.GetTotalIdleTime()) / cfg::g_simTime << std::endl;
    }
    totalChannelUsage /= (cfg::g_simTime * cfg::g_numCarriers);

    double totalWifiThroughput = 0;
    for (const auto& ap : aps)
    {
        totalWifiThroughput += 8.0 * ap.GetRxBytes() / cfg::g_simTime;
    }
    totalWifiThroughput /= cfg::g_numCarriers;

    std::cout << cfg::g_run << '\t'
              << cfg::g_simTime / 1000000 << '\t'
              << cfg::g_numCarriers << '\t'
              << cfg::g_numEnbs << '\t'
              << cfg::g_txLock << '\t'
              << totalThroughput << '\t'
              << throughputPerCell << '\t'
              << std::sqrt(perCellVariance) << '\t'
              << LbtTypeToStr(cfg::g_lbtType) << '\t'
              << cfg::g_randomPrimaryCarrier << '\t'
              << totalChannelUsage << '\t'
              << totalSuccessRatio << '\t'
              << totalWifiThroughput << '\t'
              << cfg::g_numWifiCarriers << '\t'
              << cfg::g_smart << '\t'
              << std::endl;

    return 0;
}
