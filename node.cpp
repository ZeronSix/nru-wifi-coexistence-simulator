#include "node.hpp"
#include "frame.hpp"

int Node::s_nodeCounter = 0;

void Node::Receive(Ptr<Frame> frame, Ptr<Channel> channel)
{
    if (frame->receiver.get() != this)
    {
        return;
    }

    if (channel->GetState() == Channel::State::BUSY)
    {
        m_rxBytes += frame->payload;
        ++m_rxPackets;
    }
}
