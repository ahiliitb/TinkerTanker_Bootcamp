#pragma once

#include "base_types.h"
#include "client/connection.h"
#include "json/update.h"
#include "bot.h"
#include "map.h"
#include "ringbuffer.h"
#include <random>
#include <vector>
#include "json/position.h"
#include "json/resource_chunk.h"
#include "resource_manager.h"

namespace miningbots {

class Sim;
struct Bot;
  
class Player {

using Position =  json::Position;
using ResourceChunk = json::ResourceChunk;
  
public:
  Sim &sim;
  const std::string team_name;
  const Key_t key;
  PlayerId id() const { return id_; }
  
  Player(Sim &s,
	 std::string tn,
	 size_t key_num,
	 ix::OnMessageCallback callback);
  // I will store references to player, so copy and move constructors should be disabled
  Player (const Player&) = delete;
  Player& operator= (const Player&) = delete;
  
  void pushUpdate(const json::Update &update);
  void step();
  
  // temporary function for testing
  // bots randomly scan or move
  void do_random_thing(std::default_random_engine &rng);

  std::string update_list_string();
  Energy_t move_energy_at_pos(const Position &p);
  
  // scan manager functions
  Position findUnexploredNearby(const Position &p, Bot &bot) { return scan_manager.findUnexploredNearby(p, bot); }
  std::optional<Position> findUnexplored_scan(const Position &p) { return scan_manager.findUnexplored_scan(p, map); }
  
  // fill the path in path using bfs / A* etc
  template<typename R1, typename R2>
  void findPath(R1 srcs, R2 dests, std::vector<Position> &path) {
    map.findPath<R1, R2>(srcs, dests, path);
  }

  bool checkBresenham(Position a, Position b) {
    return map.checkBresenham(a, b);
  }

  void notify_sent_scan_request(Position pos) {
    scan_manager.notify_sent_scan_request(pos);
  }

  auto get_reachable_in_circle_for_loc(Position p, unsigned int radius) {
    return map.get_reachable_in_circle_for_loc(p, radius);
  }

  void unassign_resource(Position res) {
    resource_manager.unassign_resource(res);
  }

  std::vector<ResourceChunk>& getResourceChunks(Position res) {
    return resource_manager.getResourceChunks(res);
  }

  std::optional<ResourceChunk> get_most_valuable_chunk_at(Position pos) {
    return resource_manager.get_most_valuable_chunk_at(pos);
  }
  bool is_ready_for_bot;
private:
  PlayerId id_;
  Map map;
  client::Connection connection;
  RingBuffer<json::Update, 1000> updates;
  std::vector<json::Update> update_list;
  // DONOT STORE REFERENCES OF BOTS
  std::vector<Bot> bots;
  unsigned int update_count = 0;
  ScanManager scan_manager;
  ResourceManager resource_manager;
  std::vector<json::Update> curr_updates;
  void procLandUpdate(const json::Update &update);
  void procBotUpdate(const json::Update &update);
  void procJobUpdate(const json::Update &update);
  friend class Map;
  friend class Bot;
  friend class ResourceManager;
};
  
}
