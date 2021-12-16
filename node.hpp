#pragma once

#include "channel.hpp"
#include "simulator.hpp"
#include <memory>

struct Frame;

class Node : public std::enable_shared_from_this<Node> {
public:
  Node() :
      m_rxBytes(0),
      m_rxPackets(0),
      m_id(s_nodeCounter++) {}

  virtual void Configure() = 0;
  virtual void Receive(Ptr<Frame> frame, Ptr<Channel> channel);
  virtual void NotifyChannelStateChange(Channel::State prevState, Channel::State newState, Ptr<Channel> channel) = 0;
  size_t GetRxBytes() const { return m_rxBytes; }
  size_t GetRxPackets() const { return m_rxPackets; }
  int GetId() const { return m_id; }
  Ptr<Node> GetPtr() { return shared_from_this(); }
protected:
  size_t m_rxBytes;
  size_t m_rxPackets;
  int m_id;

  static int s_nodeCounter;
};

template<typename T, typename... Args>
Ptr<T> MakeNode(Args... args) {
  Ptr<T> node = std::make_shared<T>(args...);
  node->Configure();
  return node;
}