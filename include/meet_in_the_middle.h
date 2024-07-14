#pragma once

#include "json/position.h"
#include <vector>
#include "grid.h"
#include <functional>

namespace miningbots {
  
  struct Map;
  
  typedef json::Position Position;
  
  template<class Queue>
  struct MeetInTheMiddle {

    MeetInTheMiddle(unsigned int max_x, unsigned int max_y);

    // TODO: fix this using concepts
    template<typename R1, typename R2, typename CheckFun>
    void run(R1 srcs, R2 dests, std::vector<Position> &path, CheckFun can_go);

  private:
    int cur_run;
    Queue q1, q2;
    Grid<Position> parents1, parents2;
    Grid<bool> srcs_bool_map, dests_bool_map;
    Grid<int> visited1, visited2;
    std::vector<Position> srcs_vec, dests_vec;

    template<typename CheckFun>
    bool step (Position &middle,
	       Queue &q,
	       Grid<Position> &parents,
	       Grid<int> &visited,
	       Grid<int> &visited_other,
	       CheckFun can_go);
    
    void push_path(Position from,
		   Grid<bool> &to,
		   Grid<Position> &parents,
		   std::vector<Position> &path);
    
    template<typename R1, typename R2>
    bool init(R1 srcs, R2 dests, std::vector<Position> &path);

    // TODO: fix this using concepts
    void reset_bool_maps();
  };
}
