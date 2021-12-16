#pragma once

#include "node.hpp"

class UeNode : public Node {
public:
  UeNode(const std::vector<Ptr<Channel>> &carriers);
  void Configure() override;
  void Receive(Ptr<Frame> frame, Ptr<Channel> channel) override;
  void NotifyChannelStateChange(Channel::State prevState, Channel::State newState, Ptr<Channel> channel) override {};
private:
  std::vector<Ptr<Channel>> m_carriers;
};
