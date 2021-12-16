#include "config.hpp"
#include <getopt.h>
#include <cstring>

namespace cfg {

int g_run = 0;
Time g_simTime = 1e6;
size_t g_numCarriers = 1;
int g_writePhyTraces = false;
size_t g_numEnbs = 1;
int g_txLock = 0;
LbtType g_lbtType = LbtType::A1;
PrimaryType g_primaryCarrierType = PrimaryType::Fixed;
int g_writeVcdDump = false;
size_t g_numWifiStas = 1;
size_t g_numWifiCarriers = 1;
int g_smart = 0;
int g_wifiCwMin = 15;
size_t g_wifiFrameSize = WIFI_DEFAULT_FRAME_SIZE;
double g_wifiLoad = 1.0;
double g_wifiTOnMs = 2.5;
int g_leakageWidth = 32;

enum {
  CMD_OPT_RUN = 1,
  CMD_OPT_SIMTIME,
  CMD_OPT_NUMCARRIERS,
  CMD_OPT_NUMENBS,
  CMD_OPT_TXLOCK,
  CMD_OPT_LBTTYPE,
  CMD_OPT_NUMWIFISTAS,
  CMD_OPT_NUMWIFICARRIERS,
  CMD_OPT_SMART,
  CMD_OPT_WIFICWMIN,
  CMD_OPT_WIFIFRAMESIZE,
  CMD_OPT_WIFILOAD,
  CMD_OPT_WIFITONMS,
  CMD_OPT_PRIMARYTYPE,
  CMD_OPT_LEAKAGE_WIDTH
};

void ParseCommandLineArgs(int argc, char *argv[]) {
  int longIndex = 0;
  static struct option longOptions[] = {
      {"run", required_argument, nullptr, CMD_OPT_RUN},
      {"simTime", required_argument, nullptr, CMD_OPT_SIMTIME},
      {"numCarriers", required_argument, nullptr, CMD_OPT_NUMCARRIERS},
      {"writePhyTraces", no_argument, &g_writePhyTraces, true},
      {"numEnbs", required_argument, nullptr, CMD_OPT_NUMENBS},
      {"txLock", required_argument, nullptr, CMD_OPT_TXLOCK},
      {"lbtType", required_argument, nullptr, CMD_OPT_LBTTYPE},
      {"writeVcdDump", no_argument, &g_writeVcdDump, true},
      {"numWifiStas", required_argument, nullptr, CMD_OPT_NUMWIFISTAS},
      {"numWifiCarriers", required_argument, nullptr, CMD_OPT_NUMWIFICARRIERS},
      {"wifiCwMin", required_argument, nullptr, CMD_OPT_WIFICWMIN},
      {"smart", no_argument, &g_smart, true},
      {"wifiFrameSize", required_argument, nullptr, CMD_OPT_WIFIFRAMESIZE},
      {"wifiLoad", required_argument, nullptr, CMD_OPT_WIFILOAD},
      {"wifiTOnMs", required_argument, nullptr, CMD_OPT_WIFITONMS},
      {"primaryType", required_argument, nullptr, CMD_OPT_PRIMARYTYPE},
      {"leakageWidth", required_argument, nullptr, CMD_OPT_LEAKAGE_WIDTH},
      {0, 0, 0, 0}
  };

  int opt = 0;
  while ((opt = getopt_long(argc, argv, "", longOptions, &longIndex)) != -1) {
    switch (opt) {
      case 0:break;
      case CMD_OPT_RUN:g_run = atoi(optarg);
        break;
      case CMD_OPT_SIMTIME:g_simTime = std::stol(optarg) * 1e6;
        break;
      case CMD_OPT_NUMCARRIERS:g_numCarriers = std::stoul(optarg);
        break;
      case CMD_OPT_NUMENBS:g_numEnbs = std::stoul(optarg);
        break;
      case CMD_OPT_TXLOCK:g_txLock = std::stoi(optarg);
        break;
      case CMD_OPT_LBTTYPE:
        if (std::strcmp(optarg, "A1") == 0) {
          g_lbtType = LbtType::A1;
        } else if (std::strcmp(optarg, "A2") == 0) {
          g_lbtType = LbtType::A2;
        } else if (std::strcmp(optarg, "B") == 0) {
          g_lbtType = LbtType::B;
        } else if (std::strcmp(optarg, "B2") == 0) {
          g_lbtType = LbtType::B2;
        } else {
          std::cerr << "Wrong LBT type!" << std::endl;
          std::exit(EXIT_FAILURE);
        }
        break;
      case CMD_OPT_NUMWIFISTAS:g_numWifiStas = std::stoul(optarg);
        break;
      case CMD_OPT_NUMWIFICARRIERS:g_numWifiCarriers = std::stoul(optarg);
        break;
      case CMD_OPT_WIFICWMIN:g_wifiCwMin = std::stoi(optarg);
        break;
      case CMD_OPT_WIFIFRAMESIZE:g_wifiFrameSize = std::stoul(optarg);
        break;
      case CMD_OPT_WIFILOAD:g_wifiLoad = std::stod(optarg);
        break;
      case CMD_OPT_WIFITONMS:g_wifiTOnMs = std::stod(optarg);
        break;
      case CMD_OPT_PRIMARYTYPE:
        if (std::strcmp(optarg, "Fixed") == 0) {
          g_primaryCarrierType = PrimaryType::Fixed;
        } else if (std::strcmp(optarg, "Random") == 0) {
          g_primaryCarrierType = PrimaryType::Random;
        } else if (std::strcmp(optarg, "RoundRobin") == 0) {
          g_primaryCarrierType = PrimaryType::RoundRobin;
        } else {
          std::cerr << "Wrong primary type!" << std::endl;
          std::exit(EXIT_FAILURE);
        }
        break;
      case CMD_OPT_LEAKAGE_WIDTH:g_leakageWidth = std::stoi(optarg);
        break;
      case '?':std::cerr << "Wrong args!" << std::endl;
        std::exit(EXIT_FAILURE);
        break;
      default:std::cerr << "Wrong args!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
  }
}

};
