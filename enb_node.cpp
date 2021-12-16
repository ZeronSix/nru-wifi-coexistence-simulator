#include "enb_node.hpp"
#include "config.hpp"
#include "vcd_dumper.hpp"
#include "frame.hpp"
#include <algorithm>

inline std::string BackoffCounterName(int carrierId, int nodeId) {
  return "node" + std::to_string(nodeId) + "backoff" + std::to_string(carrierId);
}

EnbNode::EnbNode(const std::vector<Ptr<Channel>> &carriers, Ptr<UeNode> ue) :
    Node(),
    m_ue(ue),
    m_lssbShift(0),
    //m_navEnd(0),
    m_txLock(0),
    m_numAcks(0),
    m_numNacks(0) {
  static size_t s_enbCounter = 0;

  int primaryIdx = (cfg::g_primaryCarrierType == cfg::PrimaryType::RoundRobin) ? s_enbCounter % carriers.size() :
                   (cfg::g_primaryCarrierType == cfg::PrimaryType::Random) ? Simulator::GenerateUniformRandomVariable(0, carriers.size() - 1)
                   : 0;
  for (size_t i = 0; i < carriers.size(); ++i) {
    if (i == primaryIdx || cfg::g_lbtType == cfg::LbtType::A1 || cfg::g_lbtType == cfg::LbtType::A2) {
      m_primaryCarriers.push_back(carriers[i]);
      m_primaryCarriersMap[carriers[i]->GetId()] = carriers[i];
    } else {
      m_secondaryCarriers.push_back(carriers[i]);
    }
    m_allCarriers.push_back(carriers[i]);
  }

  ++s_enbCounter;
}

void EnbNode::Configure() {
  for (auto carrier : m_primaryCarriers) {
    std::string counterName = BackoffCounterName(carrier->GetId(), m_id);
    VcdDumper::AddVariable(counterName);
    carrier->AddListener(GetPtr());
    m_backoffCounters[carrier->GetId()] = {0, cfg::LAA_CWMIN, counterName};
    ResetBackoffCounter(carrier->GetId());
    if (carrier->GetState() == Channel::State::IDLE) {
      RescheduleAifsHandler(carrier);
    }
  }

  for (auto carrier : m_secondaryCarriers) {
    std::string counterName = BackoffCounterName(carrier->GetId(), m_id);
    m_backoffCounters[carrier->GetId()] = {0, cfg::LAA_CWMIN, counterName};
  }
}

void EnbNode::NotifyChannelStateChange(Channel::State prevState, Channel::State newState, Ptr<Channel> channel) {
  // LOG_FUNCTION();
  ASSERT(prevState != newState);

  if (prevState == Channel::State::IDLE) {
    if (m_backoffCounters[channel->GetId()].value <= 0) {
      m_backoffCounters[channel->GetId()].Reset();
    }
    Simulator::CancelIfPending(m_backoffSlotHandlers[channel->GetId()]);
  } else if (newState == Channel::State::IDLE) {
    RescheduleAifsHandler(channel);
  }
}

void EnbNode::HandleAifs(Ptr<Channel> channel) {
  LOG_FUNCTION("Node #" << GetId() << " channel #" << channel->GetId() <<);
  ASSERT(channel->CurrentTimeIsIdleFor() >= cfg::AIFS);
  //ASSERT(m_navEnd == Simulator::Now() || channel->CurrentTimeIsIdleFor() == cfg::AIFS);

  RescheduleBackoffHandler(channel);
  m_backoffCounters[channel->GetId()].Trace();
}

void EnbNode::HandleBackoffSlot(Ptr<Channel> channel) {
  LOG_FUNCTION("Node #" << GetId() << " channel #" << channel->GetId() <<);
  ASSERT(channel->CurrentTimeIsIdleFor() >= cfg::BACKOFF_SLOT);
  // ASSERT(m_backoffCounters[channel->GetId()].value > 0);

  const int smartTxLock = (channel->GetId() < cfg::g_numWifiCarriers) ? 0 : cfg::g_txLock;
  const int txLock = cfg::g_smart ? smartTxLock : cfg::g_txLock;
  if (m_backoffCounters[channel->GetId()].value > -txLock) {
    m_backoffCounters[channel->GetId()].value--;
    LOG_MESSAGE("Backoff counter #" << channel->GetId() << " on node #" << GetId() << " decreased to "
                                    << m_backoffCounters[channel->GetId()].value <<);
  }

  if (m_backoffCounters[channel->GetId()].value == -txLock) {
    if (m_startTxEvent.eventTime == Simulator::Now()) return;

    // std::cerr << channel->GetId() << std::endl;
    m_startTxEvent = Simulator::Schedule(0, [this, txLock]() {
      StartTxBurstOnAllCarriers();
    });
  }

  // LOG_MESSAGE("Backoff counter #" << channel->GetId() << " on node #" << GetId() << " skipping at "
  //                                 << m_backoffCounters[channel->GetId()].value <<);
  RescheduleBackoffHandler(channel);
}

void EnbNode::StartTxBurst(Ptr<Channel> channel) {
  LOG_FUNCTION("Node #" << GetId() << " channel #" << channel->GetId() <<);
  if (cfg::g_lbtType != cfg::LbtType::B && cfg::g_lbtType != cfg::LbtType::B2) {
    ASSERT(channel->CurrentTimeIsIdleFor() >= cfg::AIFS);
  }

  Time remToNextLssb = cfg::LS_SLOT - (Simulator::Now() - m_lssbShift) % cfg::LS_SLOT;
  Time rsDuration = (remToNextLssb != cfg::LS_SLOT) ? remToNextLssb : 0;
  ASSERT(rsDuration < 500);

  int frameSlots = (rsDuration != 0) ? cfg::LAA_TXOP - 1 : cfg::LAA_TXOP;
  Time remainingTxop = cfg::LS_SLOT - rsDuration;
  Time lastSlotStart = Simulator::Now() + rsDuration + (cfg::LAA_TXOP - 1) * cfg::LS_SLOT;
  Time slotIndex = ((lastSlotStart - m_lssbShift) / cfg::LS_SLOT) % 2;

  static std::vector<Time> firstSlotDurations = {0, 3 * cfg::LS_SLOT / 7, 6 * cfg::LS_SLOT / 7};
  static std::vector<Time> lastSlotDurations = {0, 2 * cfg::LS_SLOT / 7, 3 * cfg::LS_SLOT / 7, 4 * cfg::LS_SLOT / 7,
                                                5 * cfg::LS_SLOT / 7, cfg::LS_SLOT};
  //lastSlotDurations = firstSlotDurations;
  const auto &durations = slotIndex ? lastSlotDurations : firstSlotDurations;

  Time lastSlotDuration = (*(--std::lower_bound(durations.begin(), durations.end(), remainingTxop)));
  //std::cout << remainingTxop << std::endl;
  // std::cout << remainingTxop / cfg::LAA_SYMBOL << " " << lastSlotDuration / cfg::LAA_SYMBOL << " " << slotIndex << std::endl;

  Time payloadDuration = (cfg::LAA_TXOP - 1) * cfg::LS_SLOT + lastSlotDuration;
  Time frameDuration = rsDuration + payloadDuration;
  ASSERT(frameDuration <= 8000);
  ASSERT(frameDuration >= 7500);
  auto frame = std::make_shared<Frame>(frameDuration,
                                       cfg::LAA_DATARATE * payloadDuration,
                                       GetPtr(),
                                       m_ue,
                                       cfg::LAA_FRAME);
  ASSERT(frame->payload / cfg::LAA_DATARATE <= frameDuration);

  for (int i = channel->GetId() - cfg::g_leakageWidth; i <= channel->GetId() + cfg::g_leakageWidth; ++i) {
    if (m_primaryCarriersMap.count(i) == 0) {
      continue;
    }
    m_navEnd[i] = std::max(Simulator::Now() + frameDuration + cfg::AIFS, m_navEnd[i]);

    if (i != channel->GetId() && m_primaryCarriersMap[i]->GetState() == Channel::State::IDLE) {
      RescheduleAifsHandler(m_primaryCarriersMap[i]);
    }
  }

  channel->Transmit(frame);
  Simulator::Schedule(0,
                       [this, channel]() {
                         m_backoffCounters[channel->GetId()].TraceInsteadOfValue(BACKOFF_COUNTER_TX_TRACE);
   });
}

void EnbNode::RescheduleAifsHandler(Ptr<Channel> channel) {
  m_backoffCounters[channel->GetId()].TraceInsteadOfValue(-m_backoffCounters[channel->GetId()].cw);

  Simulator::CancelIfPending(m_backoffSlotHandlers[channel->GetId()]);
  m_backoffSlotHandlers[channel->GetId()] = Simulator::Schedule(
      std::max<Time>(cfg::AIFS, m_navEnd[channel->GetId()] - Simulator::Now()),
      std::bind(&EnbNode::HandleAifs, this, channel)
  );
}

void EnbNode::RescheduleBackoffHandler(Ptr<Channel> channel) {
  if (Simulator::Now() < m_navEnd[channel->GetId()]) {
    RescheduleAifsHandler(channel);
    return;
  }

  Simulator::CancelIfPending(m_backoffSlotHandlers[channel->GetId()]);

  Time time = cfg::BACKOFF_SLOT;
  if (time == 0 || m_backoffSlotHandlers[channel->GetId()].eventTime != Simulator::Now()) {
    HandleBackoffSlot(channel);
  } else {
    m_backoffSlotHandlers[channel->GetId()] = Simulator::Schedule(
        time,
        //cfg::BACKOFF_SLOT,
        std::bind(&EnbNode::HandleBackoffSlot, this, channel)
    );
  }

  m_backoffCounters[channel->GetId()].Trace();
}

void EnbNode::TriggerAck(Ptr<Channel> channel) {
  LOG_FUNCTION("Node #" << GetId() <<);

  m_backoffCounters[channel->GetId()].cw = cfg::LAA_CWMIN;
  Simulator::Schedule(0, [this, channel] { ResetBackoffCounterAccordingToType(channel->GetId()); });
  // std::cout << m_backoffCounters[channel->GetId()].cw << std::endl;
  m_numAcks++;
  //ASSERT((m_numAcks + m_numNacks) == m_ue.GetRxPackets());
}

void EnbNode::TriggerNack(Ptr<Channel> channel) {
  LOG_FUNCTION("Node #" << GetId() <<);

  m_backoffCounters[channel->GetId()].Raise(cfg::LAA_CWMAX);
  Simulator::Schedule(0, [this, channel] { ResetBackoffCounterAccordingToType(channel->GetId()); });
  // std::cout << m_backoffCounters[channel->GetId()].cw << std::endl;
  m_numNacks++;
}

void EnbNode::ResetBackoffCounter(int carrierId) {
  LOG_FUNCTION();

  m_backoffCounters[carrierId].Reset();
  m_backoffCounters[carrierId].Trace();
  LOG_MESSAGE("Backoff counter #" << carrierId << " on node #" << GetId() <<
                                  " to " << m_backoffCounters[carrierId].value <<);
}

void EnbNode::ResetBackoffCounterAccordingToType(int carrierId) {
  LOG_FUNCTION();

  if (cfg::g_lbtType != cfg::LbtType::A2 && cfg::g_lbtType != cfg::LbtType::B2) {
    m_backoffCounters[carrierId].Reset();
    m_backoffCounters[carrierId].Trace();
    LOG_MESSAGE("Backoff counter #" << carrierId << " on node #" << GetId() <<
                                    " to " << m_backoffCounters[carrierId].value <<);
  } else {
    int maxCw = 0;
    for (auto carrier : m_allCarriers) {
      if (m_backoffCounters[carrier->GetId()].cw > maxCw) {
        maxCw = m_backoffCounters[carrier->GetId()].cw;
      }
    }
    for (auto carrier : m_allCarriers) {
      m_backoffCounters[carrier->GetId()].cw = maxCw;
      m_backoffCounters[carrier->GetId()].Reset();
      m_backoffCounters[carrier->GetId()].Trace();
      LOG_MESSAGE("Backoff counter #" << carrierId << " on node #" << GetId() <<
                                      " to " << m_backoffCounters[carrierId].value <<);
    }
  }
}

void EnbNode::StartTxBurstOnAllCarriers() {
  for (auto carrier : m_primaryCarriers) {
    if (m_backoffCounters[carrier->GetId()].value <= 0) {
      if (carrier->CurrentTimeIsIdleFor() >= cfg::AIFS) {
        StartTxBurst(carrier);
      }
    }
  }
  for (auto carrier : m_secondaryCarriers) {
    if (carrier->CurrentTimeIsIdleFor() >= cfg::PIFS) {
      StartTxBurst(carrier);
    }
  }
}
