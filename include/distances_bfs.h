#pragma once

#include "json/position.h"
#include "grid.h"
#include <queue>

namespace miningbots {

  struct Map;
  using Position = json::Position;
  
  struct DistancesBFS {

  public:
    DistancesBFS(unsigned int max_x, unsigned int max_y);

    template<typename R1, typename CanGoFun>
    void recalculate(R1 srcs, CanGoFun can_go);

    void print() {
      std::cout << "distances " << std::endl;
      distances.print<[](const unsigned int &x) {
	if (x == std::numeric_limits<unsigned int>::max())
	  std::cout << 0 << " ";
	else
	  std::cout << 1 << " "; }>();
      std::cout << std::endl;
    }
    
    unsigned int operator[](Position goal) const;

  private:
    
    template<typename R1>
    void init(R1 srcs);
    
    std::queue<Position> q;
    unsigned int curr_run = 0;
    Grid<unsigned int> run_when_assigned;
    Grid<unsigned int> distances;
  };
}

namespace miningbots {

  template<typename R1, typename CanGoFun>
  void DistancesBFS::recalculate(R1 srcs, CanGoFun can_go) {

    init(srcs);

    while (q.size() > 0) {
      auto p = q.front();
      q.pop();
      for (auto [pos, dist]: distances.square_indexed(p, 1)) {
	if (!can_go(pos) ||
	    run_when_assigned[pos] == curr_run) continue;
	dist = distances[p] + 1;
	q.push(pos);
	run_when_assigned[pos] = curr_run;
      }
    }
  }

  template<typename R1>
  void DistancesBFS::init(R1 srcs) {
    curr_run ++;  
    for (Position src: srcs) {
      q.push(src);
      distances[src] = 0;
      run_when_assigned[src] = curr_run;
    }
  }
  
}
