#include "channel.hpp"
#include "config.hpp"
#include "node.hpp"
#include "frame.hpp"
#include <fstream>

int Channel::s_channelId = 0;

Channel::~Channel()
{
    LOG_FUNCTION();

    if (cfg::g_writePhyTraces)
    {
        auto filename = "phyState_" + std::to_string(m_id) + ".tsv";
        std::ofstream stream;
        stream.open(filename, std::ios::out);

        stream
            << "time"     << '\t'
            << "node"     << '\t'
            << "duration" << '\t'
            << "endtime"  << '\n';

        for (const auto &it : m_phyTraces)
        {
            Time time = it.first;
            Ptr<Node> node = it.second.first;
            Time duration = it.second.second;

            stream
                << time          << '\t'
                << node->GetId() << '\t'
                << duration      << '\t'
                << time + duration << '\n';
        }

        stream.close();
    }
}

void Channel::Transmit(Ptr<Frame> frame)
{
    LOG_FUNCTION("Transmitting from " << Simulator::Now() << " to " << Simulator::Now() + frame->duration << " on " << m_id << );
    ASSERT(frame->duration > 0);

    // We need to handle all remaining endtxs before starting a new transmission
    Simulator::Schedule(0, std::bind(&Channel::DoTransmit, this, frame));
}

void Channel::SetState(Channel::State newState)
{
    LOG_FUNCTION("Channel " << this << " state changed from " <<
                 static_cast<int>(m_state) << " to " << static_cast<int>(newState) << );

    if (newState == m_state)
    {
        return;
    }

    if (newState == State::IDLE)
    {
        m_lastIdleSetTime = Simulator::Now();
    }
    else if (m_state == State::IDLE)
    {
        m_lastIdleEndTime = Simulator::Now();
        m_totalIdleTime += (m_lastIdleEndTime - m_lastIdleSetTime);
    }

    auto prevState = m_state;
    m_state = newState;

    VcdDumper::TraceVariable("channel" + std::to_string(m_id), static_cast<int>(m_state));

    for (const auto& node : m_listeners) {
        //Simulator::Schedule(0, std::bind(&Node::NotifyChannelStateChange, node, state, m_state));
        node->NotifyChannelStateChange(prevState, m_state, shared_from_this());
    }
}

bool Channel::WasIdleDuring(Time time) const
{
    LOG_FUNCTION();
    ASSERT(Simulator::Now() >= m_lastIdleSetTime);

    if (m_state != State::IDLE && m_lastIdleEndTime != Simulator::Now())
    {
        return false;
    }

    return (Simulator::Now() - m_lastIdleSetTime) >= time;
}

Time Channel::CurrentTimeIsIdleFor() const
{
    ASSERT(m_lastIdleSetTime <= Simulator::Now());

    if (m_state != State::IDLE && m_lastIdleEndTime != Simulator::Now())
    {
        return 0;
    }

    return Simulator::Now() - m_lastIdleSetTime;
}

void Channel::DoTransmit(Ptr<Frame> frame)
{
    LOG_FUNCTION();

    m_txCount++;
    if (m_txCount == 1)
    {
        ASSERT(m_state == State::IDLE);
        SetState(State::BUSY);
    }
    else
    {
        ASSERT(m_state == State::BUSY || m_state == State::COLLISION);
        SetState(State::COLLISION);
    }

    auto it = m_txs.emplace(Transmission{Simulator::Now(), Simulator::Now() + frame->duration, frame});
    Simulator::Schedule(frame->duration, std::bind(&Channel::EndTx, this, it));
    m_phyTraces.insert({Simulator::Now(), {frame->sender, frame->duration}});
}

void Channel::EndTx(TxSet::iterator txIt)
{
    LOG_FUNCTION();

    auto frame = txIt->frame;

    m_txs.erase(txIt);
    m_txCount--;

    for (auto listener : m_listeners)
    {
        listener->Receive(frame, shared_from_this());
    }

    if (m_txs.empty() || m_txs.begin()->start > Simulator::Now())
    {
        SetState(State::IDLE);
    }
}
