#pragma once

#include <queue>
#include <vector>
#include <functional>
#include <cmath>
#include "json/position.h"

namespace miningbots {

using Position = json::Position;

template <typename edge_weight_function>
class Dijkstra {
  
  static_assert(std::is_convertible_v<edge_weight_function,
		std::function<double(const Position&, const Position&)>>,
		"edge_weigth_function must be of type double(const Position&, const Position&)");
  
public:
  Dijkstra(edge_weight_function ewf, size_t max_x, size_t max_y);

  template<typename R1>
  void init(const R1 &src);

  double get_dist(Position pos);

  // void push(const Position &pos);

  // Position pop();

  size_t size() const;

private:

  std::vector<Position> sources;
  Grid<unsigned int> curr_run;
  Grid<double> distances;
  
  std::priority_queue<Position, std::vector<Position>, edge_weight_function> pq;
  
};

} // namespace miningbots

#include "priority_queue.tpp"
