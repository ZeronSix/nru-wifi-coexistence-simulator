#pragma once

#include "simulator.hpp"
#include "wifi_sta_node.hpp"

class OnOffSwitcher {
public:
  OnOffSwitcher(Time onDuration, Time offDuration, std::function<void(bool)> func, Time startOffset);

  void ToggleOff();
  void ToggleOn();
private:
  Time m_onDuration;
  Time m_offDuration;
  std::function<void(bool)> m_func;
};
