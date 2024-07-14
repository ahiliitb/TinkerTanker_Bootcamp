#pragma once

#include "base_types.h"
#include "json/position.h"
#include <vector>
#include <limits>
#include "json/resource_chunk.h"
#include <optional>

namespace miningbots {

  struct Bot;
  using json::Position;
  using json::ResourceChunk;
  struct ResourceManager;
  
  struct ResourceLoc {
    
    double base_value_sum;
    Microsecond base_time_sum;
    Energy_t base_energy_sum;

    double normalized_value;
    double normalized_time;

    std::vector<std::pair<double, int>> bot_value_time_ratio_and_index;

    void add_bots(const std::vector<Bot> &bots, ResourceManager &manager, Position pos);

    std::optional<std::pair<double, int>> get_bot_to_assign(std::vector<bool> &bots_taken, double threshold);

    bool is_assigned = false;

    // sort these by move valueable chunk first
    // (the one with the id which is the most valuable)
    std::vector<ResourceChunk> chunks;
    
    ResourceLoc() = default;
    void calc_value_time_and_energy(ResourceManager &manager, int n_bots);
    
  };
}
