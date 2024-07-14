#include "priority_queue.h"

namespace miningbots {

template <auto edge_weight_function, auto heuristic_function>
void PriorityQueue<edge_weight_function, heuristic_function>::init(const Position &src) {
  while (!pq.empty()) pq.pop(); // Clear the priority queue
  target = dst;
  push(src, 0.0);
}

template <auto edge_weight_function, auto heuristic_function>
void PriorityQueue<edge_weight_function, heuristic_function>::push(const Position &value) {
  double priority = cost + heuristic(value);
  pq.emplace(value, priority);
}

template <auto edge_weight_function, auto heuristic_function>
Position PriorityQueue<edge_weight_function, heuristic_function>::pop() {
  Node<T> top = pq.top();
  pq.pop();
  return top.value;
}

template <typename T, auto edge_weight_function, auto heuristic_function>
size_t PriorityQueue<T, edge_weight_function, heuristic_function>::size() const {
  return pq.size();
}

template <typename T, auto edge_weight_function, auto heuristic_function>
double PriorityQueue<T, edge_weight_function, heuristic_function>::heuristic(const T &value) const {
  return heuristic_function(value, target) ;
}

} // namespace miningbots
