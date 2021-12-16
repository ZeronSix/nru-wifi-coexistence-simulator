#include "on_off_switcher.hpp"

OnOffSwitcher::OnOffSwitcher(Time onDuration, Time offDuration, std::function<void(bool)> func, Time startOffset) :
    m_onDuration(onDuration),
    m_offDuration(offDuration),
    m_func(func) {
  Simulator::Schedule(
      startOffset,
      std::bind(&OnOffSwitcher::ToggleOn, this)
  );
}

void OnOffSwitcher::ToggleOff() {
  m_func(false);
  Simulator::Schedule(
      m_offDuration,
      std::bind(&OnOffSwitcher::ToggleOn, this)
  );
}

void OnOffSwitcher::ToggleOn() {
  m_func(true);
  Simulator::Schedule(
      m_onDuration,
      std::bind(&OnOffSwitcher::ToggleOff, this)
  );
}
