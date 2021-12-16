#include "wifi_ap_node.hpp"
#include "frame.hpp"

WifiApNode::WifiApNode(Ptr<Channel> channel) :
    m_channel(channel) {
}

void WifiApNode::Receive(Ptr<Frame> frame, Ptr<Channel> channel) {
  LOG_FUNCTION();
  Node::Receive(frame, channel);

  if (frame->receiver.get() != this) return;

  auto ackFrame = std::make_shared<Frame>(
      cfg::WIFI_ACK_DURATION,
      0,
      GetPtr(),
      frame->sender,
      cfg::WIFI_FRAME_ACK
  );
  Simulator::Schedule(
      cfg::SIFS,
      std::bind(&Channel::Transmit, channel, ackFrame)
  );
}
