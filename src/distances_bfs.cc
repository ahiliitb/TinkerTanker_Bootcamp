#include "distances_bfs.h"

namespace miningbots {
  
  DistancesBFS::DistancesBFS(unsigned int max_x, unsigned int max_y):
    run_when_assigned(max_x, max_y, 0),
    distances(max_x, max_y, std::numeric_limits<unsigned int>::max())
  { }


  unsigned int DistancesBFS::operator[](Position goal) const {
    return ((run_when_assigned[goal] != curr_run) ? std::numeric_limits<unsigned int>::max() : distances[goal]);
  }
}
