#pragma once

#include "meet_in_the_middle.h"
#include <iostream>

namespace miningbots {

  template<class Queue>
  MeetInTheMiddle<Queue>::MeetInTheMiddle(unsigned int max_x, unsigned int max_y) :
    cur_run(0),
    parents1(max_x, max_y),
    parents2(max_x, max_y),
    srcs_bool_map(max_x, max_y, false),
    dests_bool_map(max_x, max_y, false),
    visited1(max_x, max_y, 0),
    visited2(max_x, max_y, 0)
  { }

  template<class Queue>
  template<typename CheckFun>
  bool MeetInTheMiddle<Queue>::step (Position &middle,
				     Queue &q,
				     Grid<Position> &parents,
				     Grid<int> &visited,
				     Grid<int> &visited_other,
				     CheckFun can_go) {
    Position p = q.pop();
    for (auto [neighbor, visit_num] : visited.square_indexed(p, 1)) {
      if (!can_go(neighbor) || visit_num == cur_run) continue;
      parents[neighbor] = p;
      visit_num = cur_run;
      q.push(neighbor);
      if (visited_other[neighbor] == cur_run) {
	middle = neighbor;
	return true;
      }
    }
    return false;
  }
  
  template<class Queue>
  void MeetInTheMiddle<Queue>::push_path(Position from,
					 Grid<bool> &to,
					 Grid<Position> &parents,
					 std::vector<Position> &path) {
    while (true) {
      path.push_back(from);
      if (to[from]) {
	break;
      }
      from = parents[from];
    }
  }

  template<class Queue>
  void MeetInTheMiddle<Queue>::reset_bool_maps() {
    for (Position pos: srcs_vec) {
      srcs_bool_map[pos] = false;
    }
    for (Position pos: dests_vec) {
      dests_bool_map[pos] = false;
    }
    srcs_vec.clear();
    dests_vec.clear();
  }

  
  template<class Queue>
  template<typename R1, typename R2>
  bool MeetInTheMiddle<Queue>::init(R1 srcs, R2 dests, std::vector<Position> &path) {
    cur_run ++;
    std::cout << "sources: ";
    for (Position pos: srcs) {
      std::cout << "(" << pos.x << ", " << pos.y << "), ";
      visited1[pos] = cur_run;
      srcs_bool_map[pos] = true;
      srcs_vec.push_back(pos);
    }
    std::cout << std::endl;
    std::cout << "destinations: ";
    for (Position pos: dests) {
      std::cout << "(" << pos.x << ", " << pos.y << "), ";
      if (srcs_bool_map[pos]) {
	path.push_back(pos);
	reset_bool_maps();
	std::cout << "found common" << std::endl;
	return true;
      }
      dests_bool_map[pos] = true;
      dests_vec.push_back(pos);
      visited2[pos] = cur_run;
    }
    std::cout << std::endl;
    q1.init(srcs);
    q2.init(dests);
    return false;
  }
  
  template<class Queue>
  template<typename R1, typename R2, typename CheckFun>
  void MeetInTheMiddle<Queue>::run(R1 srcs, R2 dests, std::vector<Position> &path, CheckFun can_go) {

    path.clear();
    if (init(srcs, dests, path)) return;
      
    Position middle;
    
    while (q1.size() > 0 &&
	   !step(middle, q1, parents1, visited1, visited2, can_go) &&
	   q2.size() > 0 &&
	   !step(middle, q2, parents2, visited2, visited1, can_go)) { }

    // no path found
    if (q1.size() == 0 || q2.size() == 0) {
      std::cout << "no path found" << std::endl;
      return;
    }

    push_path(middle, srcs_bool_map, parents1, path);
    reverse(path.begin(), path.end());
    if (dests_bool_map[middle] == false)
      push_path(parents2[middle], dests_bool_map, parents2, path);

    std::cout << "path found: ";
    for (Position pos: path) {
      std::cout << "(" << pos.x << ", " << pos.y << "), ";
    }
    std::cout << std::endl;
    reset_bool_maps();
    
  }
}
