#pragma once

#include <queue>
#include "json/position.h"

namespace miningbots {

using Position = json::Position;

struct QueueBFS {
  
  std::queue<Position> q;
  QueueBFS() = default;

  template<typename R1>
  void init(R1 srcs) {
    q = {};
    for (Position pos: srcs)
      q.push(pos);
  }
  
  void push(const Position &src) {
    q.push(src);
  }
  Position pop() {
    Position p = q.front();
    q.pop();
    return p;
  }
  size_t size() {
    return q.size();
  }
};
  
}
