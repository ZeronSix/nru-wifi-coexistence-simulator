#include "ue_node.hpp"
#include "enb_node.hpp"
#include "frame.hpp"

UeNode::UeNode(const std::vector<Ptr<Channel>> &carriers) :
    m_carriers(carriers) {
}

void UeNode::Configure() {
  for (auto carrier : m_carriers) {
    carrier->AddListener(GetPtr());
  }
}

void UeNode::Receive(Ptr<Frame> frame, Ptr<Channel> channel) {
  ASSERT(channel->GetState() != Channel::State::IDLE);
  Node::Receive(frame, channel);

  if (frame->receiver.get() != this) {
    return;
  }

  if (channel->GetState() == Channel::State::BUSY) {
    std::dynamic_pointer_cast<EnbNode>(frame->sender)->TriggerAck(channel);
  } else {
    std::dynamic_pointer_cast<EnbNode>(frame->sender)->TriggerNack(channel);
  }
}
