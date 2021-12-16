#pragma once

#include "simulator.hpp"
#include <string>

namespace cfg {

enum class LbtType {
  A1,
  A2,
  B,
  B2
};

inline std::string LbtTypeToStr(LbtType t) {
  switch (t) {
    case LbtType::A1: return "A1";
    case LbtType::A2: return "A2";
    case LbtType::B: return "B";
    case LbtType::B2: return "B2";
    default: return "Unknown";
  }
}

enum class PrimaryType {
  Fixed,
  RoundRobin,
  Random
};

inline std::string PrimaryTypeToStr(PrimaryType t) {
  switch (t) {
    case PrimaryType::Fixed: return "fixed";
    case PrimaryType::RoundRobin: return "roundrobin";
    case PrimaryType::Random: return "random";
    default: return "Unknown";
  }
}

extern int g_run;
extern Time g_simTime;
extern size_t g_numCarriers;
extern int g_writePhyTraces;
extern size_t g_numEnbs;
extern int g_txLock;
extern LbtType g_lbtType;
extern PrimaryType g_primaryCarrierType;
extern int g_writeVcdDump;
extern size_t g_numWifiStas;
extern size_t g_numWifiCarriers;
extern int g_smart;
extern int g_wifiCwMin;
extern size_t g_wifiFrameSize;
extern double g_wifiLoad;
extern double g_wifiTOnMs;
extern int g_leakageWidth;

constexpr int BACKOFF_SLOT = 9;
constexpr int AIFS = 43;
constexpr int PIFS = 25;
constexpr int SIFS = 16;
constexpr int DIFS = 34;

constexpr Time LS_SLOT = 500;
constexpr int LAA_CWMIN = 15;
constexpr int LAA_CWMAX = 63;
constexpr int LAA_TXOP = 16; // slots
constexpr int LAA_DATARATE = 75; // mbps
constexpr int LAA_SYMBOL = LS_SLOT / 7;

constexpr int WIFI_CWMIN = 15;
constexpr int WIFI_CWMAX = 1023;
constexpr Time   WIFI_TXOP  = 2500;
constexpr size_t WIFI_DATARATE = 75;
constexpr size_t WIFI_DEFAULT_FRAME_SIZE = 2500 * WIFI_DATARATE / 8;
constexpr Time WIFI_ACK_DURATION = 44;
constexpr Time WIFI_ACK_TIMEOUT = SIFS + BACKOFF_SLOT;

constexpr int EIFS = SIFS + DIFS + WIFI_ACK_DURATION;

enum { WIFI_FRAME_DATA, WIFI_FRAME_ACK, LAA_FRAME };

void ParseCommandLineArgs(int argc, char *argv[]);

};
