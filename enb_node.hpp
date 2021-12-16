#pragma once

#include "node.hpp"
#include "ue_node.hpp"
#include "config.hpp"
#include "dcf_base.hpp"
#include <vector>

class EnbNode : public Node {
  friend class UeNode;
public:
  EnbNode(const std::vector<Ptr<Channel>> &carriers, Ptr<UeNode> ue);
  void Configure() override;
  void Receive(Ptr<Frame> frame, Ptr<Channel> channel) override {}
  void NotifyChannelStateChange(Channel::State prevState, Channel::State newState, Ptr<Channel> channel) override;

  double GetSuccessRatio() const { return 1.0 * m_numAcks / (m_numAcks + m_numNacks); }
private:
  std::vector<Ptr<Channel>> m_primaryCarriers;
  std::map<int, Ptr<Channel>> m_primaryCarriersMap;
  std::vector<Ptr<Channel>> m_secondaryCarriers;
  std::vector<Ptr<Channel>> m_allCarriers;
  std::map<int, Event> m_backoffSlotHandlers;
  std::map<int, BackoffCounter> m_backoffCounters;
  Ptr<UeNode> m_ue;
  Time m_lssbShift;
  std::map<int, Time> m_navEnd;
  int m_txLock;
  size_t m_numAcks;
  size_t m_numNacks;
  Event m_startTxEvent;

  void HandleAifs(Ptr<Channel> channel);
  void HandleBackoffSlot(Ptr<Channel> channel);
  void StartTxBurst(Ptr<Channel> channel);
  void RescheduleAifsHandler(Ptr<Channel> channel);
  void RescheduleBackoffHandler(Ptr<Channel> channel);
  void TriggerAck(Ptr<Channel> channel);
  void TriggerNack(Ptr<Channel> channel);
  void ResetBackoffCounter(int carrierId);
  void ResetBackoffCounterAccordingToType(int carrierId);
  void StartTxBurstOnAllCarriers();
};
