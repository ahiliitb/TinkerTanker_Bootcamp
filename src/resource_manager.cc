#include "resource_manager.h"
#include "player.h"
#include "sim.h"
#include "json/resource_config.h"
#include <iostream>
#include <algorithm>
#include <ratio>
#include <spdlog/spdlog.h>
#include <queue>

namespace miningbots {

  using json::ResourceConfig;
  using json::ResourceChunk;
  using json::MapConfig;
  using Bot = Bot;

  ResourceManager::ResourceManager(Sim &s)
    : sim(s),
      resources(sim.max_x(), sim.max_y()),
      resources_values(sim.num_resources(), 0),
      valuable_resources(sim.num_resources(),0),
      curr_resources(sim.num_resources(), 0),
      resource_dag(sim.max_x(), sim.max_y()),
      start_time(std::chrono::steady_clock::now())
  { 
    for (ResourceChunk r : sim.map_config.action_buildbot_resource_cost){
      valuable_resources[r.id] = 1;
    }
    for (ResourceChunk r : sim.map_config.win_condition){
      valuable_resources[r.id] = 1;
    }
  }

  void ResourceManager::procLandUpdate(const json::LandUpdate &land_update) {
    Position pos = land_update.position;
    resources[pos].chunks = land_update.resources;
  }
   bool ResourceManager::is_build_bot_possible(Player & player){
    for(miningbots::Bot& bot: player.bots){
      if (!bot.is_minining_bot()){
        if (bot.energy()<player.sim.map_config.action_buildbot_energy){
          return 0;
        }
      } 
    }
    for (ResourceChunk r : player.sim.map_config.action_buildbot_resource_cost){
      if (curr_resources[r.id]<r.amount){
        return 0;
      }
    }
    return 1;
  }

  bool ResourceManager::is_benificial_to_build_bot(Player & player){
    int number_of_bots = player.bots.size();
    if (number_of_bots<=7){
      return 1;
    }
    auto now = std::chrono::steady_clock::now();
    Microsecond curr_time = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time).count();
    long double time_required_without_bot = 0;
    for (ResourceChunk r : player.sim.map_config.win_condition){
      if (curr_resources[r.id] == 0){
        return 1;
      }
      long double time_required_for_r = r.amount;
      time_required_for_r /= curr_resources[r.id]; //time required for that resource
      time_required_without_bot = std::max(time_required_without_bot,time_required_for_r);
    }
    time_required_without_bot*=curr_time;
    long double time_required_with_bot = 0;
    std::vector<Kilogram> changed_resource = curr_resources;
    for (ResourceChunk r : player.sim.map_config.action_buildbot_resource_cost){
      changed_resource[r.id]-=r.amount;
    }
    for (ResourceChunk r : player.sim.map_config.win_condition){
      long double speed = curr_resources[r.id];
      speed /= curr_time;
      long double time_required_for_r = speed;
      time_required_for_r *= (curr_time + player.sim.map_config.action_buildbot_interval);
      time_required_for_r -= curr_resources[r.id];
      time_required_for_r += changed_resource[r.id];
      if (time_required_for_r > r.amount){
        continue;
      }
      time_required_for_r = r.amount - time_required_for_r;
      time_required_for_r /= speed;
      time_required_for_r *= number_of_bots;
      time_required_for_r /= number_of_bots + 1;
      time_required_with_bot = std::max(time_required_with_bot, time_required_for_r);
    }
    // std::cout<<time_required_with_bot<<"-------------------"<<time_required_without_bot<<std::endl;
    time_required_with_bot += player.sim.map_config.action_buildbot_interval;
    
    return time_required_with_bot*number_of_bots <= time_required_without_bot;
  }
  void ResourceManager::instruct_mining_bots_to_deliver(Player & player){
    std::vector<Kilogram> resources_required(curr_resources.size(),0);
    for (ResourceChunk r : player.sim.map_config.action_buildbot_resource_cost){
      resources_required[r.id] += r.amount;
    }
    for (miningbots::Bot & bot : player.bots){
      if (!bot.is_minining_bot()){
        for (ResourceChunk r :bot.cargo() ){
          if (resources_required[r.id] > r.amount){
            resources_required[r.id] -= r.amount;
          }
          else{
            resources_required[r.id] = 0;
          }
        }
      }
    }
    Position p;
    BotId id;
    for (miningbots::Bot & bot : player.bots){
      if (!bot.is_minining_bot()){
        p = bot.position();
        id = bot.id();
        break;
      }
    }
    double bot_factory_close = 0;
    Bot * current = & player.bots[0];
    bool bo = 1;
    while (bo){
      for (miningbots::Bot & bot : player.bots){
        if (bot.is_minining_bot()){
          double is_bot_going = 0;
          for (ResourceChunk r :bot.cargo() ){
            if (r.amount > 0 && resources_required[r.id] > 0){
              if (resources_required[r.id] > r.amount){
                is_bot_going += (double) r.amount/(double) resources_required[r.id];
              }
              else{
                is_bot_going ++;
              }
            }
            is_bot_going /= bot.distance_to(p);
            if (is_bot_going > bot_factory_close){
              bot_factory_close = is_bot_going;
              current = & bot;
            }
          }
        }
      }
      for (ResourceChunk r :current -> cargo() ){
        if (r.amount > 0 && resources_required[r.id] > 0){
          if (resources_required[r.id] > r.amount){
            resources_required[r.id] -= r.amount;
          }
          else{
            resources_required[r.id] = 0;
          }
        }
      }
      current->go_to_factory(p,id);
      bo = 0;
      for (Kilogram r : resources_required ){
        if (r > 0){
          bo = 1;
          break;
        }
      }
    }
  }
  double ResourceManager::calculate_chunk_value(ResourceChunk chunk) const {
    // can do better approximation later, if amount is too much that we need to reduce value
    return resources_values[chunk.id] * chunk.amount;
  }
  
  Microsecond ResourceManager::calculate_chunk_time(ResourceChunk chunk) const {
    const ResourceConfig &res_conf = sim.map_config.resource_configs[chunk.id];
    return res_conf.mine_interval * ((chunk.amount + res_conf.mine_amount_per_interval - 1) / res_conf.mine_amount_per_interval);
  }
  
  Energy_t ResourceManager::calculate_chunk_energy(ResourceChunk chunk) const {
    const ResourceConfig &res_conf = sim.map_config.resource_configs[chunk.id];
    return res_conf.mine_energy_per_interval * ((chunk.amount + res_conf.mine_amount_per_interval - 1) / res_conf.mine_amount_per_interval);
  }
  
  Microsecond ResourceManager::calculate_chunk_normalized_time(ResourceChunk chunk) const {
    return normalize_time_from_time_and_energy(calculate_chunk_time(chunk), calculate_chunk_energy(chunk));
  }
  
  double ResourceManager::calculate_chunks_value(const std::vector<ResourceChunk> &chunks) const {
    double val = 0;
    for (const ResourceChunk &chunk : chunks) {
      val += calculate_chunk_value(chunk);
    }
    return val;
  }

  Microsecond ResourceManager::calculate_chunks_time(const std::vector<ResourceChunk> &chunks) const {
    Microsecond val = 0;
    for (const ResourceChunk &chunk : chunks) {
      val += calculate_chunk_time(chunk);
    }
    return val;
  }

  Energy_t ResourceManager::calculate_chunks_energy(const std::vector<ResourceChunk> &chunks) const {
    Energy_t val = 0;
    for (const ResourceChunk &chunk : chunks) {
      val += calculate_chunk_energy(chunk);
    }
    return val;
  }

  void ResourceManager::sort_chunks_by_value(std::vector<ResourceChunk> &chunks) const {
    sort(chunks.begin(), chunks.end(), [&](const ResourceChunk &a, const ResourceChunk &b) {
      return resources_values[a.id] > resources_values[b.id];
    });
  }

  // returns the maximum value / time that we can get
  // ensure that chunks are sorted by value before this
  double ResourceManager::calculate_value_for_bot_x_time_away(ResourceLoc &resloc, Microsecond bot_time_to_reach_resource) {
    std::vector<ResourceChunk> &chunks = resloc.chunks;
    if (chunks.size() == 0) {
      return 0;
    }
    else {
      return resources_values[chunks[0].id] / bot_time_to_reach_resource;
    }
  }

  double ResourceManager::calculate_value_for_bot_and_resource(Position pos, ResourceLoc &resloc, const Bot &bot) {
    Microsecond time_to_reach = bot.time_to_reach_position(pos);
    return calculate_value_for_bot_x_time_away(resloc, time_to_reach);
  }
  
  void ResourceManager::calc_curr_resources(std::vector<Bot> &bots) {
    for (Kilogram &amount: curr_resources) {
      amount = 0;
    }
    for (Bot &bot: bots) {
      for (const ResourceChunk &chunk : bot.cargo()) {
	curr_resources[chunk.id] += chunk.amount;
      }
    }
  }

  void ResourceManager::calc_resource_values(const std::vector<Kilogram> &req_resource_amounts,Player & player) {
    resources_amount_left = 0;
    for (size_t i = 0; i < curr_resources.size(); i ++) {
      Kilogram left = 0;
      if (req_resource_amounts[i] > curr_resources[i])
	left = req_resource_amounts[i] - curr_resources[i];
      if (left == 0){
        resources_values[i] = 0;
      }
      else{
        resources_values[i] = ((double) left) / req_resource_amounts[i] ;
        resources_amount_left += left;
      }
    }
    if (player.bots.size() < 6){
      for (json::ResourceChunk r : sim.map_config.action_buildbot_resource_cost){
        if (curr_resources[r.id] == 0){
          resources_values[r.id]++;
          resources_values[r.id] *= 100;
        }
        else if (curr_resources[r.id] < r.amount){
          resources_values[r.id] *= ((double) r.amount)/((double) curr_resources[r.id]);
        }
        else{
          resources_values[r.id] /= 100;
        }
      }     
    }
  }
 
  void ResourceManager::refresh_after_bot_updates(std::vector<Bot> &bots) {
    calc_curr_resources(bots);
    calc_resource_values(sim.req_resource_amounts,bots[0].player);
  }

  void ResourceManager::refresh_after_land_updates(const Map &map, size_t n_bots) {
    scanned_resources.clear();
    reachable_resources.clear();
    for (auto [pos, resloc] : resources.cells_indexed()) {
      if (resloc.chunks.size() > 0 && map.is_minable(pos)) {
	reachable_resources.push_back(pos);
	resloc.calc_value_time_and_energy(*this, n_bots);
	resource_value_sum += resloc.base_value_sum;
      }
      scanned_resources.push_back(pos);
    }
    compute_normalized();
  }

  Microsecond ResourceManager::normalize_time_from_time_and_energy(Microsecond base_time, Energy_t base_energy) const {
    // energy per microsecond = energy_recharge_per_interval / energy_recharge_interval
    return base_time + sim.time_from_energy(base_energy);
  }

  void ResourceManager::compute_normalized() {

    // reset dag for every node in node_list
    for (Position p: node_list) {
      resource_dag[p].parents.clear();
      resource_dag[p].children.clear();
      resource_dag[p].level = std::numeric_limits<unsigned int>::max();
    }
    
    node_list.clear();
    
    for (Position p: reachable_resources) {
      node_list.push_back(p);
      resource_dag[p].level = 0;
    }
    
    // construct the DAG
    unsigned int iter = 0;
    while (iter < node_list.size()) {
      Position pos = node_list[iter];
      unsigned int c_level = resource_dag[pos].level;
      for (auto [neighbor, node] : resource_dag.circle_indexed(pos, 1)) {
	if (resources[neighbor].chunks.size() == 0) continue;
	if (node.level <= c_level) continue;
	if (node.level == std::numeric_limits<unsigned int>::max())
	  node_list.push_back(neighbor);
	node.level = c_level + 1;
	node.parents.push_back(pos);
	resource_dag[neighbor].children.push_back(pos);
      }
      iter ++;
    }

    // reset the normalized values and times
    for (Position p: node_list) {
      resources[p].normalized_value = 0;
      resources[p].normalized_time = 0;
    }

    // go backwards in the node list, add for every parent
    for (auto it = node_list.rbegin(); it != node_list.rend(); it ++) {
      ResourceLoc &rl = resources[*it];
      rl.normalized_value += rl.base_value_sum;
      rl.normalized_time += normalize_time_from_time_and_energy(rl.base_time_sum, rl.base_energy_sum);
      size_t num_pars = resource_dag[*it].parents.size();
      if (rl.normalized_value == 0) continue; // do not wan't to add to times of parents in this case
      for (Position parent: resource_dag[*it].parents) {
	resources[parent].normalized_value += rl.normalized_value / num_pars;
	resources[parent].normalized_time += rl.normalized_time / num_pars;
      }
    }
  }

  double ResourceManager::get_threshold_value() {
    return 0;
  }

  void ResourceManager::unassign_resource(Position p) {
    resources[p].is_assigned = false;
  }

  void ResourceManager::assign_resources(Player &player, unsigned int num_idle_bots) {

    // implementation idea:
    // value = calculate_value_for_bot_and_resource
    // assign resource with maximum value
    for (Position &pos: reachable_resources) {
      resources[pos].add_bots(player.bots, *this, pos);
    }

    // highest first
    sort(reachable_resources.begin(), reachable_resources.end(), [&](const Position &a, const Position &b){
      return resources[a].bot_value_time_ratio_and_index[0].first > resources[b].bot_value_time_ratio_and_index[0].first;
    });
    
    std::vector<bool> bots_taken(player.bots.size(), false);

    unsigned int curr_idle_bots = player.bots.size() - 1;
    unsigned int bot_idx = 0;
    for (Bot &bot: player.bots) {
      if (bot.has_resource_assigned()) {
	Position res = bot.assigned_resource.value();
	if (bot.can_mine(res)) {
	  // check if resource has value
	  if (resources[res].base_value_sum > 0) {
	    resources[res].is_assigned = true;
	    bots_taken[bot_idx] = true;
	    curr_idle_bots --;
	  }
	}
      }
      bot_idx ++;
      if (curr_idle_bots <= num_idle_bots) break;
    }

    double threshold_value = get_threshold_value();

    for (Position res: reachable_resources) {
      std::optional<std::pair<double, int>> opt = resources[res].get_bot_to_assign(bots_taken, threshold_value);
      if (resources[res].is_assigned) continue;
      if (opt.has_value()) {
	curr_idle_bots --;
	auto [value_time_ratio, bot_idx] = opt.value();
	resources[res].is_assigned = true;
	SPDLOG_INFO("bot id: {}, assigned resource {}, {}, with vrt {}, base_value, {}", player.bots[bot_idx].id(), res.x, res.y, value_time_ratio, resources[res].base_value_sum);
	player.bots[bot_idx].assign_resource(res, value_time_ratio);
      }
      if (curr_idle_bots <= num_idle_bots) break;
    }
  }

  std::optional<ResourceChunk> ResourceManager::get_most_valuable_chunk_at(Position pos) {
    double max_value = -1;
    std::optional<ResourceChunk> max_value_resource = std::nullopt;
    for (auto chunk : resources[pos].chunks) {
      if (resources_values[chunk.id] > max_value) {
	max_value = resources_values[chunk.id];
	max_value_resource = std::optional<ResourceChunk>(chunk);
      }
    }
    return max_value_resource;
  }


  void ResourceManager::print() {
    
    std::cout << "resource base value: " << std::endl;
    resources.print<[](const ResourceLoc &resloc) { std::cout << resloc.base_value_sum << " ";}>();
    std::cout << std::endl;
    
    std::cout << "resource base time: " << std::endl;
    resources.print<[](const ResourceLoc &resloc) { std::cout << resloc.base_time_sum << " ";}>();
    std::cout << std::endl;
    
    std::cout << "resource base energy: " << std::endl;
    resources.print<[](const ResourceLoc &resloc) { std::cout << resloc.base_energy_sum << " ";}>();
    std::cout << std::endl;
    
    std::cout << "resource normalized value: " << std::endl;
    resources.print<[](const ResourceLoc &resloc) { std::cout << resloc.normalized_value << " ";}>();
    std::cout << std::endl;
    
    std::cout << "resource normalized time: " << std::endl;
    resources.print<[](const ResourceLoc &resloc) { std::cout << resloc.normalized_time << " ";}>();
    std::cout << std::endl;
    
  }
  
}
