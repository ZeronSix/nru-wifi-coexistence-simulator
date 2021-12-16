#pragma once

#include <set>
#include <limits>
#include <memory>
#include "simulator.hpp"
#include "vcd_dumper.hpp"

struct Frame;

struct Transmission {
  Time start;
  Time end;
  Ptr<Frame> frame;
};

struct TxCmpByStart {
  bool operator()(const Transmission &lhs, const Transmission &rhs) const {
    return (lhs.start != rhs.start) ? lhs.start < rhs.start : lhs.end < rhs.end;
  }
};

class Node;

class Channel : public std::enable_shared_from_this<Channel> {
public:
  enum class State {
    IDLE,
    BUSY,
    COLLISION
  };

  enum class MacState {
    TX,
    AIFS,
    BACKOFF
  };

  Channel() :
      m_id{++s_channelId},
      m_txCount(0),
      m_state(State::IDLE),
      m_lastIdleSetTime(0),
      m_lastIdleEndTime(std::numeric_limits<int>::min()),
      m_totalIdleTime(0) {
    VcdDumper::AddVariable("channel" + std::to_string(m_id));
  }
  Channel(const Channel &) = delete;
  ~Channel();

  int GetId() const { return m_id; }
  void Transmit(Ptr<Frame> frame);
  State GetState() const { return m_state; }
  void SetState(Channel::State newState);
  bool WasIdleDuring(Time time) const;
  Time CurrentTimeIsIdleFor() const;

  void AddListener(Ptr<Node> node) {
    m_listeners.push_back(node);
  }

  Time LastIdleEndTime() const { return m_lastIdleEndTime; }
  Time GetTotalIdleTime() const { return m_totalIdleTime; }
private:
  using TxSet = std::multiset<Transmission, TxCmpByStart>;

  int m_id;
  TxSet m_txs;
  unsigned m_txCount;
  State m_state;
  Time m_lastIdleSetTime;
  Time m_lastIdleEndTime;
  Time m_totalIdleTime;

  std::vector<Ptr<Node>> m_listeners;
  std::multimap<Time, std::pair<Ptr<Node>, Time>> m_phyTraces;
  // std::map<std::pair<int, Time>, std::string> m_macTraces;

  void DoTransmit(Ptr<Frame> frame);
  void EndTx(TxSet::iterator txIt);

  static int s_channelId;
};
