#include "wifi_sta_node.hpp"
#include "wifi_ap_node.hpp"
#include "config.hpp"
#include "frame.hpp"

WifiStaNode::WifiStaNode(Ptr<Channel> channel, Ptr<WifiApNode> apNode) :
    Node(),
    m_channel(channel),
    m_backoffCounter{0,
                     cfg::WIFI_CWMIN,
                     "wifiNode" + std::to_string(m_id) + "backoff" + std::to_string(channel->GetId())},
    m_apNode(apNode),
    m_lastTxEnd(-1),
    m_lastEifsEnd(-1),
    m_txing(false),
    m_on(true) {
}

void WifiStaNode::Configure() {
  VcdDumper::AddVariable(m_backoffCounter.traceName);

  m_channel->AddListener(GetPtr());

  m_backoffCounter.Reset();
  if (m_channel->GetState() == Channel::State::IDLE) {
    RescheduleAifsHandler();
  }
}

void WifiStaNode::Receive(Ptr<Frame> frame, Ptr<Channel> channel) {
  if (m_lastTxEnd >= Simulator::Now()) return;

  if (channel->GetState() != Channel::State::BUSY && frame->type != cfg::LAA_FRAME) {
    m_lastEifsEnd = Simulator::Now() + cfg::EIFS;
    return;
  }

  Node::Receive(frame, channel);

  if (frame->receiver.get() != this) return;
  ASSERT(frame->type == cfg::WIFI_FRAME_ACK);

  m_txing = false;
  Simulator::CancelIfPending(m_ackTimeoutHandler);
  m_backoffCounter.cw = cfg::WIFI_CWMIN;
  m_backoffCounter.Reset(); // post-backoff
  m_backoffCounter.Trace();
}

void WifiStaNode::NotifyChannelStateChange(Channel::State prevState, Channel::State newState, Ptr<Channel> channel) {
  ASSERT(channel == m_channel);
  ASSERT(prevState != newState);

  if (prevState == Channel::State::IDLE) {
    Simulator::CancelIfPending(m_backoffSlotHandler);
  } else if (newState == Channel::State::IDLE) {
    if (m_ackTimeoutHandler.eventTime < Simulator::Now()) {
      RescheduleAifsHandler();
    }
  }
}

void WifiStaNode::RescheduleAifsHandler() {
  m_backoffCounter.TraceInsteadOfValue(-m_backoffCounter.cw);

  Simulator::CancelIfPending(m_backoffSlotHandler);
  Time time = std::max<Time>({cfg::AIFS - m_channel->CurrentTimeIsIdleFor(), 0, m_lastEifsEnd - Simulator::Now()});
  m_backoffSlotHandler = Simulator::Schedule(
      time,
      std::bind(&WifiStaNode::HandleAifs, this)
  );
}

void WifiStaNode::RescheduleBackoffHandler() {
  ASSERT(m_backoffCounter.value != 0);
  Simulator::CancelIfPending(m_backoffSlotHandler);

  Time time = cfg::BACKOFF_SLOT;
  m_backoffSlotHandler = Simulator::Schedule(
      cfg::BACKOFF_SLOT,
      std::bind(&WifiStaNode::HandleBackoffSlot, this)
  );

  m_backoffCounter.Trace();
}

void WifiStaNode::HandleAifs() {
  LOG_FUNCTION("Node #" << GetId() << " channel #" << m_channel->GetId() <<);
  ASSERT(m_channel->CurrentTimeIsIdleFor() >= cfg::AIFS);
  if (m_ackTimeoutHandler.eventTime >= Simulator::Now()) {
    return;
  }

  if (!m_on) return;

  if (m_backoffCounter.value != 0) {
    RescheduleBackoffHandler();
    m_backoffCounter.Trace();
  } else {
    Transmit();
  }
}

void WifiStaNode::HandleBackoffSlot() {
  LOG_FUNCTION("Node #" << GetId() << " channel #" << m_channel->GetId() <<);
  ASSERT(m_channel->CurrentTimeIsIdleFor() >= cfg::BACKOFF_SLOT);
  ASSERT(m_ackTimeoutHandler.eventTime < Simulator::Now());
  ASSERT(m_backoffCounter.value > 0);

  m_backoffCounter.value--;
  LOG_MESSAGE("Backoff counter #" << m_channel->GetId() << " on node #" << GetId() << " decreased to "
                                  << m_backoffCounter.value <<);
  m_backoffCounter.Trace();

  if (m_backoffCounter.value == 0) {
    Transmit();
    return;
  }

  RescheduleBackoffHandler();
}

void WifiStaNode::HandleAckTimeout() {
  LOG_FUNCTION("Node #" << GetId() << " channel #" << m_channel->GetId() <<);

  m_txing = false;
  m_backoffCounter.Raise(cfg::WIFI_CWMAX);
  m_backoffCounter.Reset();
  if (m_channel->GetState() == Channel::State::IDLE) {
    RescheduleAifsHandler();
  }
}

void WifiStaNode::Transmit() {
  LOG_FUNCTION("Node #" << GetId() <<);

  auto payload = cfg::g_wifiFrameSize;
  auto duration = GetWifiFrameDuration(payload);
  auto frame = std::make_shared<Frame>(duration, payload, GetPtr(), m_apNode, cfg::WIFI_FRAME_DATA);

  m_channel->Transmit(frame);
  m_backoffCounter.TraceInsteadOfValue(BACKOFF_COUNTER_TX_TRACE);
  m_ackTimeoutHandler = Simulator::Schedule(duration + cfg::WIFI_ACK_TIMEOUT,
                                            std::bind(&WifiStaNode::HandleAckTimeout, this));
  m_lastTxEnd = Simulator::Now() + duration;
}

void WifiStaNode::SwitchOn(bool value) {
  m_on = value;

  if (!m_txing && m_channel->WasIdleDuring(cfg::AIFS)) {
    HandleAifs();
  }
}
