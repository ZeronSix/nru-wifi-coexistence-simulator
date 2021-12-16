#include "simulator.hpp"
#include "config.hpp"
#include "vcd_dumper.hpp"

Time Simulator::s_simTime = 0;
Time Simulator::s_nowTime = 0;
EventQueue Simulator::s_eventQueue;
std::mt19937 Simulator::s_engine;

void Simulator::Configure()
{
    LOG_FUNCTION();

    s_simTime = cfg::g_simTime;
    s_engine.seed(cfg::g_run);
}

Event Simulator::Schedule(Time time, const std::function<void ()>& callback)
{
    static int eventId = 0;

    Event e{++eventId, s_nowTime + time, s_nowTime, callback};
    s_eventQueue.insert(std::pair<Time, Event>(e.eventTime, e));

    return e;
}

void Simulator::Run()
{
    LOG_FUNCTION();

    while (!s_eventQueue.empty() && s_nowTime <= s_simTime)
    {
        auto it = s_eventQueue.begin();
        if (it->first > s_simTime)
        {
            s_eventQueue.clear();
            break;
        }

        s_nowTime = it->first;

        auto callback = it->second.callback;
        s_eventQueue.erase(it);
        callback();
    }

    VcdDumper::Dump("vcd_dump.vcd");
}

void Simulator::CancelIfPending(Event& e)
{
    if (e.eventTime == s_nowTime) return; // don't cancel events that are scheduled to happen now

    for (auto it = s_eventQueue.lower_bound(e.eventTime); it != s_eventQueue.upper_bound(e.eventTime); it++)
    {
        if (it->second.id == e.id)
        {
            ASSERT(it->second.eventTime == e.eventTime);
            s_eventQueue.erase(it);
            return;
        }
    }

    e = Event();
}

int Simulator::GenerateUniformRandomVariable(int min, int max)
{
    std::uniform_int_distribution<> distribution{min, max};

    return distribution(s_engine);
}
