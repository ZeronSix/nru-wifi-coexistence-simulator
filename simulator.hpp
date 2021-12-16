#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <random>
#include <iostream>
#include <utility>
#include <memory>

#ifndef NDEBUG
#define LOG_FUNCTION(smth) do { std::cerr << Simulator::Now() << " " << __PRETTY_FUNCTION__ << " " << smth " \n"; } while (0)
#define LOG_MESSAGE(smth) do { std::cerr << Simulator::Now() << " [LOG] " << __PRETTY_FUNCTION__ << " " << smth " \n"; } while (0)
#else
#define LOG_FUNCTION(smth) do {} while (0)
#define LOG_MESSAGE(smth) do {} while (0)
#endif

#define FATAL_ERROR(msg) do { std::cerr << "FATAL ERROR: " << msg << "\n"; std::terminate(); } while (0)

#define ASSERT(cond) \
    {\
        if (!(cond)) \
        { \
            std::cerr << Simulator::Now() << " " << __FILE__ << ":" << __LINE__ << " Assertion '" #cond "' failed.\n"; \
            std::terminate(); \
        } \
    }

using Time = int64_t;
template<typename T>
using Ptr = std::shared_ptr<T>;

struct Event
{
    int  id;
    Time eventTime;
    Time schedulingTime;
    std::function<void ()> callback;

    Event(): id(-1), eventTime(-1), schedulingTime(-1) {}
    Event(int id, Time eventTime, Time schedulingTime, std::function<void ()>  cb):
        id(id),
        eventTime(eventTime),
        schedulingTime(schedulingTime),
        callback(std::move(cb)) {}
};

using EventQueue = std::multimap<Time, Event>;

class Simulator
{
public:
    static void  Configure();
    static Event Schedule(Time time, const std::function<void ()>& callback);
    static void  Run();
    static void  CancelIfPending(Event& e);
    static int   GenerateUniformRandomVariable(int min, int max);

    static Time Now() { return s_nowTime; }
private:
    static Time s_nowTime;
    static Time s_simTime;

    static EventQueue s_eventQueue;
    static std::mt19937 s_engine;
};
