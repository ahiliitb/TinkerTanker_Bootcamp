#pragma once

#include "base_types.h"
#include "json/map_config.h"
#include "json/resource_config.h"
#include <vector>
#include "json/update.h"
#include <ixwebsocket/IXWebSocket.h>
#include "assert.h"
#include "json/game_constants.h"

namespace miningbots {
  
  struct Player;
  
class Sim {
      
public:
  
  const std::string config_path;
  const json::GameConstants game_constants;
  const std::string host_name;
  const int port;
  const std::vector<Key_t> valid_player_keys;
  GameId game_id;
  const json::MapConfig map_config;
  const std::vector<Kilogram> req_resource_amounts;
  
  std::vector<std::unique_ptr<Player>> players;
  double harmonic_mean_speed_in_all_terrains;
    
  Sim (std::string cp);
  // I will store references to sim, so copy and move constructors should be disabled
  Sim (const Sim&) = delete;
  Sim& operator= (const Sim&) = delete;
  
  Microsecond time_from_energy(Energy_t energy) const {
    return (Microsecond) ((energy * map_config.energy_recharge_interval) / map_config.energy_recharge_per_interval);
  }
  
  // TODO: approximate according to the harmonic mean speed
  Microsecond distance_to_time(Kilometer dist) {
    return (Microsecond) (dist / harmonic_mean_speed_in_all_terrains);
  }

  void calc_harmonic_mean_speed();
  
  void run();
  void procUpdate(const json::Update &update);
  Kilometer max_x() const { return map_config.max_x; };
  Kilometer max_y() const { return map_config.max_y; };
  size_t num_resources() const { return getResourceTypes().size(); };
  Key_t get_key(size_t idx) const { assert(idx < valid_player_keys.size()); return valid_player_keys[idx]; }
  static void messageCallback(Sim &sim, const ix::WebSocketMessagePtr &message);
  void addPlayer(std::string name, size_t key_num, const ix::OnMessageCallback &cb);
  Rarity_t getResourceRarity(ResourceId id) const { return getResourceTypes()[id].rarity; };
  const std::vector<json::ResourceConfig>& getResourceTypes() const { return map_config.resource_configs; }
  const std::vector<json::TerrainConfig>& getTerrainTypes() const { return map_config.terrain_configs; }
  
private:
  json::GameConstants readGameConstants();
  json::MapConfig readMapConfig();
  std::vector<Key_t> readPlayerKeys();
  std::vector<Kilogram> getReqResourceAmounts();
  GameId find_game();
  void restart_game();
  void set_map();
  bool print_to_file;
  std::string updates_output_file;
};
  
  
};
