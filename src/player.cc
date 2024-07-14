#include "player.h"
#include <ixwebsocket/IXWebSocket.h>
#include "json/join_game_response.h"
#include "json/join_game_request.h"
#include "json/client_subscribe_request.h"
#include "json/get_games_response.h"
#include "json/game_status.h"
#include <cpr/cpr.h>
#include <glaze/json/write.hpp>
#include <utils.h>
#include "sim.h"
#include <algorithm>
#include "glaze/glaze.hpp"

#include "gui/gui.h"

namespace miningbots {
  
Player::Player(Sim &s,
	       std::string tn,
	       size_t pkey,
	       ix::OnMessageCallback callback)
  : sim(s),
    team_name(tn),
    key(s.get_key(pkey)),
    map(s),
    connection(sim.host_name, sim.port, "update", callback),
    updates(),
    scan_manager(s),
    resource_manager(s),
    is_ready_for_bot(0)
{    
  using namespace miningbots;
  
  json::JoinGameRequest join_game_request{sim.game_id, team_name, key};
  auto response =
    cpr::Put(cpr::Url{fmt::format("http://{}:{}/join_game", sim.host_name, sim.port)},
	     cpr::Header{{"Content-Type", "application/json"}},
	     cpr::Parameters{
	       {"request", glz::write_json(join_game_request)},
		 });
  ThrowExceptionIfError(response.text);

  id_ = ReadJson<json::JoinGameResponse>(response.text);

  SPDLOG_INFO("          Received player_id: {} for {}", id_,
	      join_game_request.player_name);

  json::ClientSubscribeRequest client_subscribe_request{sim.game_id, key, id_};
  connection.sendWSText(glz::write_json(client_subscribe_request));
}

void Player::procLandUpdate(const json::Update &update) {
  for (const auto &lu: update.land_updates) {
    map.procLandUpdate(lu);
    resource_manager.procLandUpdate(lu);
  }
}

void Player::procJobUpdate(const json::Update &update) {
  for (const auto &ju: update.job_updates) {
    JobId id = ju.id;
    auto find_bot_by_job = [id](const Bot& b) {
      return b.current_job_id() == id;
    };
    auto it = std::find_if(bots.begin(), bots.end(), find_bot_by_job);
    if (it != bots.end())
      it->job_update(ju);
    else {} // ignore for now
  }
}
  
void Player::procBotUpdate(const json::Update &update) {
  for (const auto &bu: update.bot_updates) {
    map.procBotUpdate(bu);
    BotId id = bu.id;
    auto find_bot_by_id = [id](const Bot& b) {
      return b.id() == id;
    };
    auto it = std::find_if(bots.begin(), bots.end(), find_bot_by_id);
    if (it != bots.end())
      it->bot_update(bu);
    else
      bots.emplace_back(*this, bu);
  }
}

void Player::pushUpdate(const json::Update &update) {
  updates.push(update);
  update_list.push_back(update);
}

void Player::step() {
  
  using json::Update;
  curr_updates.clear();
  
  while (!updates.isEmpty()) {
    curr_updates.push_back(updates.pop());  
    update_count ++;
  }
  if (resource_manager.is_build_bot_possible(*this) && resource_manager.is_benificial_to_build_bot(*this)){
    resource_manager.instruct_mining_bots_to_deliver(*this);
    is_ready_for_bot = 1;
  }
  if (curr_updates.size() > 0) {

    if (sim.game_constants.show_gui) {
      for (const Update& update: curr_updates) {
	GUI::single_update(update);
	GUI::drawGrid(GUI::botPosition_map);
	SDL_RenderPresent(GUI::gRenderer);
      }
    }
  
    for (const Update& update: curr_updates) {
      procBotUpdate(update);
    }
    
    resource_manager.refresh_after_bot_updates(bots);
    if (!resource_manager.resources_left()) {
      Position factory_loc;
      BotId factory_id;
      for (Bot& bot: bots) {
        if (!bot.is_minining_bot()) {
          factory_loc = bot.position();
          factory_id = bot.id();
        }
      }
      for (Bot& bot: bots) {
        if (bot.is_minining_bot()) {
          bot.go_to_factory(factory_loc, factory_id);
        }
      }
    }
    
    for (const Update& update: curr_updates) {
      procLandUpdate(update);
    }
    
    resource_manager.refresh_after_land_updates(map, bots.size());

    for (Bot &bot : bots) {
      bot.refresh();
    }

    double scan_percentage = scan_manager.get_scan_percentage();
    double bots_percentage = scan_percentage / sim.game_constants.scan_bots_fraction;
    unsigned int num_scan_bots = (unsigned int)((bots.size() - 1) * bots_percentage);
    
    resource_manager.assign_resources(*this, num_scan_bots);
    scan_manager.refresh_from_map(map, update_count);
    
    for (const Update& update: curr_updates) {
      procJobUpdate(update);
    }
    
    // map.print();
    // resource_manager.print();
    // scan_manager.print(); 
  }
  
  // for each bot do stuff
  for (Bot& bot: bots) {
    // bot.distances.print();
    if (bot.is_minining_bot())
      bot.do_mining_bot_work();
    else
      bot.do_factory_bot_work();
  } 
  
}

void Player::do_random_thing(std::default_random_engine &rng) {
  for (Bot &bot: bots) {
    bot.do_random_thing(rng);
  }
}
  
std::string Player::update_list_string() {
  std::string buffer{};
  std::cout << "update list size: " << update_list.size() << std::endl;
  glz::write_json(update_list, buffer);
  return buffer;
}

Energy_t Player::move_energy_at_pos(const Position &p) {
  if (!map.is_scanned(p)) return 0;
  return sim.map_config.terrain_configs[map.get_terrain_at_pos(p)].move_energy_per_interval;
}
  
}
