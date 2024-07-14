#pragma once

#include "resource_loc.h"
#include "json/land_update.h"
#include <limits>
#include "grid.h"

namespace miningbots {

  struct Map;
  struct Player;
  struct Sim;
  
  struct ResourceManager {
    
    ResourceManager(Sim &s);
    void procLandUpdate(const json::LandUpdate &land_update);
    void refresh_after_bot_updates(std::vector<Bot> &bots);
    void refresh_after_land_updates(const Map &map, size_t n_bots);
    void assign_resources(Player &player, unsigned int num_idle_bots);
    void print();
    void unassign_resource(Position p);
    bool resources_left() {
      return resources_amount_left > 0;
    }
    std::chrono::steady_clock::time_point start_time;
    std::vector<ResourceChunk>& getResourceChunks(Position pos) {
      return resources[pos].chunks;
    }
    bool is_build_bot_possible(Player & player);
    bool is_benificial_to_build_bot(Player & player);
    void instruct_mining_bots_to_deliver(Player & player);
    std::optional<ResourceChunk> get_most_valuable_chunk_at(Position pos);

    
  private:
    void calc_curr_resources(std::vector<Bot> &bots);
    void calc_resource_values(const std::vector<Kilogram> &req_resource_amounts,Player & player);
    Microsecond normalize_time_from_time_and_energy(Microsecond base_time, Energy_t base_energy) const;
    void compute_normalized();

    double calculate_chunk_value(ResourceChunk chunk) const;
    Microsecond calculate_chunk_time(ResourceChunk chunk) const;
    Energy_t calculate_chunk_energy(ResourceChunk chunk) const;
    Microsecond calculate_chunk_normalized_time(ResourceChunk chunk) const;
    double calculate_chunks_value(const std::vector<ResourceChunk> &chunks) const;
    Microsecond calculate_chunks_time(const std::vector<ResourceChunk> &chunks) const;
    Energy_t calculate_chunks_energy(const std::vector<ResourceChunk> &chunks) const;
    void sort_chunks_by_value(std::vector<ResourceChunk> &chunks) const;
    double calculate_value_for_bot_x_time_away(ResourceLoc &resloc, Microsecond bot_time_to_reach_resource);
    double calculate_value_for_bot_and_resource(Position pos, ResourceLoc &resloc, const Bot &bot);

    const Sim &sim;
    Grid<ResourceLoc> resources;
    std::vector<Position> scanned_resources;  
    std::vector<Position> reachable_resources;
    std::vector<bool> valuable_resources;
    double resource_value_sum;
    unsigned int resources_amount_left;
    std::vector<double> resources_values;
    std::vector<Kilogram> curr_resources;
    
    struct ResourceDAGNode {
      std::vector<Position> parents;
      std::vector<Position> children;
      unsigned int level = std::numeric_limits<unsigned int>::max();
    };
    Grid<ResourceDAGNode> resource_dag;
    std::vector<Position> node_list;

    double get_threshold_value();
    
    friend class ResourceLoc;
    friend class Bot;
  };
}
