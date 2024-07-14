#pragma once

#include "base_types.h"
#include "json/bot_update.h"
#include "json/job_update.h"
#include "json/resource_chunk.h"
#include <optional>
#include <random>
#include <cpr/cpr.h>
#include <queue>
#include <iostream>
#include "distances_bfs.h"
#include "resource_manager.h"


namespace miningbots {

class Player;
  
struct Bot {

  typedef json::Position Position;
  typedef json::ResourceChunk ResourceChunk;
  
  Bot(Player &p, const json::BotUpdate &b);
  void bot_update(const json::BotUpdate &b);
  void job_update(const json::JobUpdate &j);
  BotId id() const { return last_update.id; };

  // TODO: some actions can only be performed by some bots
  //       create different classes for each bot variant
  void move(Position p);
  void mine(Position p, ResourceChunk target_resource);
  void scan(Position p);
  void explode();
  void transfer(Position p, Id target_bot, ResourceChunk target_chunk);
  void build_bot();
  Position position() const { return last_update.position; }
  Energy_t energy() const { return last_update.current_energy; }
  JobId current_job_id() const { return last_update.current_job_id; }
  std::vector<ResourceChunk>& cargo() { return last_update.cargo; }
  bool is_minining_bot() const { return last_update.variant == json::Variant::kMiningBot; }

  bool follow_path();
  void do_factory_bot_work();
  void do_mining_bot_work();
  void do_exploring_work();
  void do_resource_collection_work();
  void do_factory_transfer_work();

  // TODO: store value time ratio
  void assign_resource(Position pos, double value_time_ratio) {
    if (factory_position.has_value()) return;
    (void) value_time_ratio;
    assigned_resource = std::optional<Position>(pos);
    if (!does_path_lead_to(pos))
      reset_path();
  }

  void go_to_factory(Position fpos, BotId fid) {
    factory_position = std::optional<Position>(fpos);
    factory_bot_id = fid;
    if (!does_path_lead_to(fpos))
      reset_path();
  }

  bool does_path_lead_to(Position pos) {
    if (position_i_am_going_to == pos) return true;
    return false;
  }

  bool has_resource_assigned() const {
    return assigned_resource.has_value();
  }

  Position going_to_position() const {
    if (assigned_resource.has_value())
      return assigned_resource.value();
    if (last_move_command_pos.has_value())
      return last_move_command_pos.value();
    else
      return position();
  }
  
  std::optional<Position> last_move_command_pos;

  void deal_async_response();
  
  // temporary function for testing
  // returns true if did something
  // does either a scan or a move
  // both random
  bool do_random_thing(std::default_random_engine &rng);

  // returns false if we are busy with a job
  bool can_do_job();

  unsigned int distance_to(Position pos) const {
    return distances[pos];
  }

  bool can_reach(Position pos) const {
    return distances[pos] != std::numeric_limits<unsigned int>::max();
  }

  void refresh();

  Microsecond time_to_reach_position(Position pos) const;
  int cnt = 0 ;
  
  bool can_mine(Position resource);
  
private:
  unsigned int update_number = 1;
  unsigned int low_energy_at_update = 0;
  
  Player &player;
  json::BotUpdate last_update;
  json::JobUpdate last_job_update;

  
  // the path that need to be followed by the bot
  std::vector<Position> path;
  size_t current_pos_in_path;
  size_t next_position_in_path;

  // response to cpr::PostAsync
  std::unique_ptr<cpr::AsyncResponse> async_response;
  bool has_pending_post;
  bool is_running_job;

  // move towards mining your assignpped resource
  std::optional<Position> assigned_resource;

  std::optional<Position> factory_position;
  BotId factory_bot_id;

  DistancesBFS distances;

  Position position_i_am_going_to;
  
  void send_request(const std::string &request_str, const std::string &request_type);  
  template<typename R1> void findPath(R1 targets);
  void findMiningPath(Position resource);
  std::optional<Position> getNextMove();
  void energy_too_low();
  void reset_path();
  bool can_transfer(Position other);

  friend class Player;
  friend class ResourceManager;
};

}
