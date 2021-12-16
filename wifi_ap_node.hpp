#pragma once

#include "node.hpp"

class WifiApNode : public Node
{
public:
    explicit WifiApNode(Ptr<Channel> channel);

    void Configure() override { m_channel->AddListener(GetPtr()); }
    void Receive(Ptr<Frame> frame, Ptr<Channel> channel) override;
    void NotifyChannelStateChange(Channel::State prevState, Channel::State newState, Ptr<Channel> channel) override {}
private:
    Ptr<Channel> m_channel;
};
