#include "sim.h"
#include "utils.h"
#include "json/error_response.h"
#include "json/get_games_response.h"
#include <random>
#include <algorithm>
#include "cpr/cpr.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include "player.h"
#include "json/game_constants.h"
#include "json/create_map_request.h"
#include "gui/gui.h"

namespace miningbots {
  
json::MapConfig Sim::readMapConfig() {
  auto response =
    cpr::Get(cpr::Url{fmt::format("http://{}:{}/map_config", host_name, port)},
	     cpr::Header{{"Content-Type", "application/json"}},
	     cpr::Parameters{{"game_id", glz::write_json(game_id)}});
  ThrowExceptionIfError(response.text);
  return ReadJson<json::MapConfig>(response.text);
}
  
std::vector<Key_t> Sim::readPlayerKeys() {
  auto str_buffer = ReadFile(config_path + "player_keys.json");
  return ReadJson<std::vector<Key_t>>(str_buffer);
}
  
json::GameConstants Sim::readGameConstants() {
  auto str_buffer = ReadFile(config_path + "game_constants.json");
  return ReadJson<json::GameConstants>(str_buffer);
}
  
void Sim::messageCallback(Sim &sim, const ix::WebSocketMessagePtr &message) {
  if (message->type == ix::WebSocketMessageType::Message) {
    // SPDLOG_INFO("       received ws:{}", message->str);
    if (message->str.starts_with("{\"err")) {
      auto error_response =
	ReadJson<json::ErrorResponse>(message->str);
    } else if (message->str.starts_with("{\"upd")) {
      auto update = ReadJson<json::Update>(message->str);
      sim.procUpdate(update);
    }
  } else if (message->type == ix::WebSocketMessageType::Fragment) {
    // no op
  } else if (message->type == ix::WebSocketMessageType::Error) {
    auto error_message = fmt::format(
				     "Error: {}\nretries: {}\nWait time(ms): {}\nHTTP Status: {}",
				     message->errorInfo.reason, message->errorInfo.retries,
				     message->errorInfo.wait_time, message->errorInfo.http_status);
    SPDLOG_ERROR(error_message);
  } else if (message->type == ix::WebSocketMessageType::Open) {
    SPDLOG_INFO("Connected {}:{}", sim.host_name, sim.port);
  } else if (message->type == ix::WebSocketMessageType::Close) {
    SPDLOG_INFO("received close");
  } else if (message->type == ix::WebSocketMessageType::Ping) {
    // no op
  } else if (message->type == ix::WebSocketMessageType::Pong) {
    // no op
  }
}

Sim::Sim(std::string cp)
  : config_path(cp),
    game_constants(readGameConstants()),
    host_name(game_constants.host_name),
    port(game_constants.port),
    valid_player_keys(readPlayerKeys()),
    game_id(find_game()),
    map_config(readMapConfig()),
    req_resource_amounts(getReqResourceAmounts())
{
  if (game_constants.restart_at_begining) {
    restart_game();
    game_id = find_game();
  }
  std::cout << map_config.resource_configs.size() << std::endl;
  calc_harmonic_mean_speed();
}

void Sim::procUpdate(const json::Update &update) {
  using enum json::UpdateType;

  PlayerId id = update.player_id;
  auto p = std::find_if(players.begin(), players.end(), [id](const std::unique_ptr<Player>& p) {  return p->id() == id; });
  if (p == players.end()) {
    // bug
    return;
  }
  
  switch (update.update_type) {
  case kGameStarted:
    break;
  case kTickUpdate:
    (*p)->pushUpdate(update);
    break;
  case kEndInWin:    
    SPDLOG_INFO("Team {} Won!", (*p)->team_name);
    break;
  case kEndInLoss:
    SPDLOG_INFO("Team {} lost :(", (*p)->team_name);
    break;
  case kEndInDraw:
    SPDLOG_INFO("Both teams drew");
    break;
  case kError:
    SPDLOG_INFO("error update");
    break;
  }
}

void Sim::addPlayer(std::string name, size_t key_num, const ix::OnMessageCallback &cb) {
  players.emplace_back(std::make_unique<Player>(*this, name, key_num, cb));
}

void Sim::run() {
  if (game_constants.show_gui) {
    GUI::initSDL();
  }
  std::default_random_engine rng{std::random_device{}()};
  const auto start{std::chrono::steady_clock::now()};
  while (true) {
    for (auto &player: players) {
      player->step();
    }
    const auto curr{std::chrono::steady_clock::now()};
    auto milisecs = std::chrono::duration_cast<std::chrono::milliseconds>(curr - start);
    if (milisecs.count() > game_constants.seconds_to_run_for * 1000) break;
  }
  if (game_constants.store_updates) {
    int idx = 0;
    for (auto &player: players) {
      std::ofstream update_file;
      update_file.open(game_constants.updates_output_file + "_" + std::to_string(idx));
      update_file << player->update_list_string() << std::endl;
      idx++;
    }
  }
  if (game_constants.show_gui) {
    GUI::closeSDL();
  }
}

std::vector<Kilogram> Sim::getReqResourceAmounts() {
  std::vector<Kilogram> req_res(getResourceTypes().size(), 0);
  for (const json::ResourceChunk &rc: map_config.win_condition) {
    req_res[rc.id] += rc.amount;
  }
  return req_res;
}

void Sim::set_map() {
  json::CreateMapRequest create_map_request{30,
							30,
							{{10, 20}},
							{{"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "},
							 {"                              "}}};
  auto response = cpr::Post(
			    cpr::Url{fmt::format("http://{}:{}/restart_game", host_name, port)},
			    cpr::Header{{"Content-Type", "application/json"}},
			    cpr::Parameters{
			      {"game_id", glz::write_json(game_id)},
			      {"create_map_request", glz::write_json(create_map_request)}
			    });
}

GameId Sim::find_game() {
  while (true) {
    auto response =
      cpr::Get(cpr::Url{fmt::format("http://{}:{}/games", host_name, port)});

    SPDLOG_INFO("{}", response.text);
    ThrowExceptionIfError(response.text);

    GameId game_id = 0;
    auto get_games_response = ReadJson<json::GetGamesResponse>(response.text);
    for (auto &g : get_games_response) {
      if (g.game_status == json::GameStatus::kNotStarted ||
	  g.game_status == json::GameStatus::kOpen) {
	game_id = g.game_id;
	break;
      }
    }
    if (game_id == 0) {
      SPDLOG_ERROR("No open games found. {}", response.text);
      continue;
    }
    SPDLOG_INFO("Found game id {}", game_id);
    return game_id;
  }
}

void Sim::restart_game() {
  auto response =
    cpr::Post(cpr::Url{fmt::format("http://{}:{}/restart_game", host_name, port)},
	      cpr::Header{{"Content-Type", "application/json"}},
	      cpr::Parameters{{"game_id", glz::write_json(game_id)}});

  // restarting the game
  SPDLOG_INFO("{}", response.text);
  ThrowExceptionIfError(response.text);
}

void Sim::calc_harmonic_mean_speed() {
  double speed_inv_sum;
  for (const json::TerrainConfig &terrain_config: map_config.terrain_configs) {
    Microsecond time_for_one_km = terrain_config.move_interval + time_from_energy(terrain_config.move_energy_per_interval);
    double speed_inv = (double) time_for_one_km;
    speed_inv_sum += speed_inv;
  }
  harmonic_mean_speed_in_all_terrains = map_config.terrain_configs.size() / speed_inv_sum;
}
  
}
