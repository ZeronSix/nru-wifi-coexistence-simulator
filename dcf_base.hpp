#pragma once

constexpr int BACKOFF_COUNTER_TX_TRACE = -2;
constexpr int BACKOFF_COUNTER_AIFS_TRACE = -1;

struct BackoffCounter
{
    int value;
    int cw;
    std::string traceName;

    void Reset()
    {
        value = Simulator::GenerateUniformRandomVariable(0, cw);
    }

    void Raise(int cwMax)
    {
        cw = std::min(2 * (cw + 1) - 1, cwMax);
    }

    void Trace()
    {
        VcdDumper::TraceVariable(traceName, value);
    }

    void TraceInsteadOfValue(int instead)
    {
        VcdDumper::TraceVariable(traceName, instead);
    }
};

