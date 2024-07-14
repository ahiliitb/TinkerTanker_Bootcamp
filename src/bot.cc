#include "bot.h"
#include "sim.h"
#include "json/action_move_request.h"
#include "json/action_mine_request.h"
#include "json/action_scan_request.h"
#include "json/action_explode_request.h"
#include "json/action_transfer_request.h"
#include "json/action_build_bot.h"
#include "player.h"
#include "spdlog/fmt/bundled/core.h"
#include <stdio.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include "json/position.h"
#include <sys/types.h>

namespace miningbots {

  typedef json::Position Position;

  Bot::Bot(Player &p, const json::BotUpdate &b)
    : player(p),
      last_update(b),
      has_pending_post(false),
      is_running_job(false),
      assigned_resource(std::nullopt),
      factory_position(std::nullopt),
      distances(p.sim.max_x(), p.sim.max_y())
  {  }

  void Bot::bot_update(const json::BotUpdate &b) {
    last_update = b;
    if (b.current_job_id == 0) {
      is_running_job = false;
    }
    update_number ++;
  }

  void Bot::deal_async_response() {
    if (async_response->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      auto response = async_response->get();
      if (response.text.starts_with("{\"err")) {
	is_running_job = false;
	// SPDLOG_INFO("response: {}", response.text);
	// auto error_response =
	//   ReadJson<json::ErrorResponse>(response_text);
      }
      has_pending_post = false;
    }
  }
  
  void Bot::job_update(const json::JobUpdate &j) {

    switch (j.action) {
    case json::Action::kMove:
      if (j.status == json::JobStatus::kCompleted) {
	current_pos_in_path = next_position_in_path;
      }
      else if (j.status == json::JobStatus::kCancelled ||
	       j.status == json::JobStatus::kCancelledPathBlocked) {
	reset_path();
      }
      break;
    default:
      // TODO add other things here as well
      break;
    }
    last_job_update = j;
    
    if (has_pending_post) {
      deal_async_response();
    }
  }
  

  template<typename R1>
  void Bot::findPath(R1 targets) {
    reset_path();
    player.findPath<>(std::ranges::single_view(std::move(position())),
			  targets, path);
  }

  template<>
  void Bot::findPath<Position>(Position target) {
    reset_path();
    player.findPath<>(std::ranges::single_view(std::move(position())),
			std::ranges::single_view(std::move(target)), path);
  }

  std::optional<Position> Bot::getNextMove() {
    if (path[current_pos_in_path] != position()) {
      // I got some error when following path!
      // maybe ran out of energy
      return std::nullopt;
    }
    // next_position_in_path = current_pos_in_path + 1;
    // return std::optional<Position>(path[next_position_in_path]);
    size_t s = current_pos_in_path;
    size_t e = std::min(path.size(), s + player.sim.game_constants.max_move_dist);
    
    while (e - s > 1) {
      size_t m = (s + e) / 2;
      if (player.checkBresenham(path[m], position()) &&
	  player.checkBresenham(position(), path[m]))
	s = m;
      else {
	e = m;
      }
    }
    next_position_in_path = s;
    return std::optional<Position>(path[s]);
  }
  
  void Bot::send_request(const std::string &request_str, const std::string &request_type) {
    // SPDLOG_INFO("Sending POST to {}:{}/{}\n{}", player.sim.host_name, player.sim.port, request_type, request_str);
    // SPDLOG_INFO("current position: {}, {}", position().x, position().y);
    // SPDLOG_INFO("maximum distance: {}", player.sim.map_config.action_scan_max_distance);
    async_response = std::make_unique<cpr::AsyncResponse>
      (cpr::PostAsync(cpr::Url{fmt::format("http://{}:{}/{}", player.sim.host_name, player.sim.port, request_type)},
		      cpr::Header{{"Content-Type", "application/json"}},
		      cpr::Parameters{{"request", request_str}}));    
    has_pending_post = true;
    is_running_job = true;
    last_move_command_pos = std::nullopt;
  }

  void Bot::energy_too_low() {
    // SPDLOG_INFO("LOW ENERGY {}", update_number);
    low_energy_at_update = update_number;
    return;
  }
  
  void Bot::move(Position p) {
    if (energy() < player.move_energy_at_pos(p)) return energy_too_low();
    json::ActionMoveRequest move_request{player.sim.game_id, player.id(), player.key, id(), p};
    send_request(glz::write_json(std::move(move_request)), "move");
    last_move_command_pos = std::optional<Position>(p);
  }

  void Bot::mine(Position p, json::ResourceChunk target_chunk) {
    if (energy() < player.sim.map_config.resource_configs[target_chunk.id].mine_energy_per_interval) return energy_too_low();
    json::ActionMineRequest mine_request{player.sim.game_id, player.id(), player.key, id(), p, target_chunk};
    send_request(glz::write_json(std::move(mine_request)), "mine");
  }

  void Bot::scan(Position p) {
    if (energy() < player.sim.map_config.action_scan_energy) return energy_too_low();
    player.notify_sent_scan_request(p);
    json::ActionScanRequest scan_request{player.sim.game_id, player.id(), player.key, id(), p};
    send_request(glz::write_json(std::move(scan_request)), "scan");
  }

  void Bot::explode() {
    json::ActionExplodeRequest explode_request{player.sim.game_id, player.id(),player.key, id()};
    send_request(glz::write_json(std::move(explode_request)), "explode");
  }
  
  void Bot::transfer(Position p, Id target_bot, ResourceChunk target_chunk) {
    json::ActionTransferRequest transfer_request{player.sim.game_id, player.id(), player.key, id(), p, target_bot, target_chunk};
    send_request(glz::write_json(std::move(transfer_request)), "transfer");
  }
  void Bot::build_bot(){
    json::ActionBuildBotRequest build_bot_request{player.sim.game_id, player.id(), player.key, id()};
    send_request(glz::write_json(std::move(build_bot_request)), "build_bot");
  }

  bool Bot::can_do_job() {
    if (has_pending_post) {
      if (async_response->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
	deal_async_response();
      }
      else return false;
    }
    if (is_running_job) {
      // SPDLOG_INFO("is already running job");
      return false;
    }
    if (energy() == 0 || low_energy_at_update == update_number) {
      // SPDLOG_INFO("low energy");
      return false;
    }
    return true;
  }

  bool Bot::do_random_thing(std::default_random_engine &rng) {
    if (!can_do_job()) return false;
    
    Position cpos = position();
    int pos_x = ((int)(rng() % 5)) - 2 + cpos.x;
    int pos_y = ((int)(rng() % 5)) - 2 + cpos.y;
    if (pos_x < 0) pos_x = 0;
    if (pos_y < 0) pos_y = 0;
    if ((size_t)pos_x >= player.sim.max_x()) pos_x = player.sim.max_x() - 1;
    if ((size_t)pos_y >= player.sim.max_y()) pos_y = player.sim.max_y() - 1;
    Position pos = {(Kilometer)pos_x, (Kilometer)pos_y};
    bool do_move = (rng() % 4);
    if (do_move == 0) {
      move(pos);
    }
    else {
      scan(pos);
    }
    return true;
  }

  bool Bot::can_mine(Position resource) {
    return positionInDistance(position(), resource, player.sim.map_config.action_mine_max_distance);
  }

  bool Bot::can_transfer(Position other) {
    return positionInDistance(position(), other, player.sim.map_config.action_transfer_max_distance);
  }

  void Bot::do_factory_transfer_work() {
    Position fac = factory_position.value();
    if (!can_transfer(fac)) {
      auto factory_transfer_locs = player.get_reachable_in_circle_for_loc(fac, player.sim.map_config.action_transfer_max_distance);
      position_i_am_going_to = fac;
      findPath(factory_transfer_locs);
      if (path.size() == 0) {
	// no path found!
	// what do?
      }
    }
    else {
      // TODO: only transfer required stuff
      std::vector<ResourceChunk>& resource_chunks = cargo();
      std::vector<ResourceChunk> valuable_resources_in_cargo;
      for (int i = 0; i < resource_chunks.size(); i++){
        if(player.resource_manager.valuable_resources[resource_chunks[i].id] == 1){
          valuable_resources_in_cargo.push_back(resource_chunks[i]);
        }
      }
      int i = 0;
      if (valuable_resources_in_cargo.size() == i) {
	// have transfered everything!
	factory_position = std::nullopt;
      }
      else {
	transfer(fac, factory_bot_id, valuable_resources_in_cargo[i]);
  i++;
      }
    }
  }

  void Bot::do_resource_collection_work() {
    Position res = assigned_resource.value();
    // SPDLOG_INFO("resource pos: {}, {}", res.x, res.y);
    if (!can_mine(res)) {
      // std::cout << "cannot mine " << std::endl;
      // std::cout << res.x << " " << res.y << std::endl;
      auto mining_locations = player.get_reachable_in_circle_for_loc(res, player.sim.map_config.action_mine_max_distance);
      position_i_am_going_to = res;
      findPath(mining_locations);
      if (path.size() == 0) {
	// no path found!
	player.unassign_resource(res);
	assigned_resource = std::nullopt;
      }
    }
    else {
      // these are already sorted by their value!
      std::vector<ResourceChunk> &chunks = player.getResourceChunks(res);
      if (chunks.size() == 0) {
	// have mined everything here!
	player.unassign_resource(res);
	assigned_resource = std::nullopt;
      }
      else {
	mine(res, chunks[0]);
      }
    }
  }

  void Bot::do_exploring_work() {
    std::optional<Position> scan_pos = player.findUnexplored_scan(position());
    if (scan_pos.has_value()) {
      scan(scan_pos.value());
    }
    else {
      Position go_pos = player.findUnexploredNearby(position(), *this);
      position_i_am_going_to = go_pos;
      findPath(go_pos);
      if (path.size() == 0) {
	// what do we even do here?
	// find unexplored nearby should return a reachable position ideally
	// for now lets just go
	move(go_pos);
      }
    }
  }

  void Bot::reset_path() {
    path.clear();
    current_pos_in_path = 0;
    next_position_in_path = 0;
  }

  bool Bot::follow_path() {
    if (path.size() > 0 && current_pos_in_path + 1 != path.size()) {
      // have to send another request!
      std::optional<Position> next_move = getNextMove();
      if (next_move.has_value()) {
	move(next_move.value());
	return true;
      }
      else {
	reset_path();
      }
    }
    return false;
  }

  void Bot::do_factory_bot_work() {

    // std::cout << "do work " << std::endl;
    // already doing a job or have sent a post which wasn't responded yet
    if (!can_do_job()) return;
    
    if (follow_path()) return;
    
    // no resource targets, get to scanning!
    if (player.is_ready_for_bot){
      player.is_ready_for_bot = 0;
      build_bot();
    }
    else if  (cnt < 30){
      do_exploring_work();
      cnt++;}
  }

  void Bot::do_mining_bot_work() {

    // already doing a job or have sent a post which wasn't responded yet

    
    if (!can_do_job()) {
      return;
    }
    

    // if (position() != Position({10, 20}))
    //   move({10, 20});
    // else
    //   move({22, 14});

    if (follow_path()) {
      return;
    }
    
    if (factory_position.has_value()) {
      do_factory_transfer_work();
    }
    else if (has_resource_assigned()) {
      // SPDLOG_INFO("resource collecting work");
      do_resource_collection_work();
    }
    else {
      // no resource targets, get to scanning!
      // SPDLOG_INFO("exploring work");
      do_exploring_work();
    }    
  }

  void Bot::refresh() {
    auto position_range = std::ranges::single_view(position());
    auto can_go = [&](const Position &pos) { return player.map.can_go(pos); };
    distances.recalculate<decltype(position_range), decltype(can_go)>(position_range, can_go);
  }

  Microsecond Bot::time_to_reach_position(Position pos) const {
    return player.sim.distance_to_time(distances[pos]);
  }
  
}
