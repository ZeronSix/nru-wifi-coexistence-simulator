#pragma once

#include "channel.hpp"
#include "dcf_base.hpp"
#include "node.hpp"

class WifiApNode;

class WifiStaNode : public Node {
public:
  WifiStaNode(Ptr<Channel> channel, Ptr<WifiApNode> ap);
  void Configure() override;

  void Receive(Ptr<Frame> frame, Ptr<Channel> channel) override;
  void NotifyChannelStateChange(Channel::State prevState, Channel::State newState, Ptr<Channel> channel) override;
  void SwitchOn(bool value);
private:
  Ptr<Channel> m_channel;
  BackoffCounter m_backoffCounter;
  Event m_backoffSlotHandler;
  Event m_ackTimeoutHandler;
  Ptr<WifiApNode> m_apNode;
  Time m_lastTxEnd;
  Time m_lastEifsEnd;
  bool m_txing;
  bool m_on;

  void RescheduleAifsHandler();
  void RescheduleBackoffHandler();
  void HandleAifs();
  void HandleBackoffSlot();
  void HandleAckTimeout();
  void Transmit();
};