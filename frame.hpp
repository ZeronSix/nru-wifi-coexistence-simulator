#pragma once

#include "simulator.hpp"
#include "config.hpp"
#include "node.hpp"
#include <memory>

struct Frame {
  Time duration;
  size_t payload;
  Ptr<Node> sender;
  Ptr<Node> receiver;
  int type;

  Frame(Time duration, size_t payload, Ptr<Node> sender, Ptr<Node> receiver, int type) :
      duration(duration),
      payload{payload},
      sender{sender},
      receiver{receiver},
      type(type) {}
};

using FramePtr = std::shared_ptr<Frame>;

inline Time GetWifiFrameDuration(size_t payload) {
  // TxTime = TPreamble + Tsignal + Tsym * Nsym
  // Nsym = ceil((16 + 8 * LENGTH + 6) / 216)
  //int nSym = std::ceil((16 + 8 * payload + 6) / 216.0);
  // return 16 + 4 + nSym * 4;
  return payload * 8 / cfg::WIFI_DATARATE;
}

