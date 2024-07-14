#include "map.h"
#include "sim.h"
#include <iostream>
#include "spdlog/fmt/bundled/core.h"
#include <spdlog/spdlog.h>


namespace miningbots {
  
  void Map::procLandUpdate(const json::LandUpdate &land_update) {
    Position pos = land_update.position;
    is_traversable[pos] = land_update.is_traversable ? Traversable::Yes : Traversable::No;
    terrain[pos] = land_update.terrain_id;
    proc_reachability(pos, land_update.is_traversable);
  }

  void Map::procBotUpdate(const json::BotUpdate &bot_update) {
    Position pos = bot_update.position;
    is_traversable[pos] = Traversable::Yes;
    if (!reachable[pos]) {
      reachable[pos] = true;
      propagate_reachability(pos);
    }
  }

  Map::Map(const Sim &s) :
    sim(s),
    reachable(s.max_x(), s.max_y(), false),
    minable_frontier(s.max_x(), s.max_y(), false),
    is_traversable(s.max_x(), s.max_y(), Traversable::Unknown),
    terrain(s.max_x(), s.max_y(), json::Terrain::kUnassigned),
    mitm_bfs(s.max_x(), s.max_y())
  {   }

  void Map::print() {
    std::cout << "map " << std::endl;
    is_traversable.print<[] (const Traversable &t) {
	  if (t == Traversable::Yes) std::cout << 1 << " ";
	  else if (t == Traversable::No) std::cout << 2 << " ";
	  else std::cout << 0 << " ";
    }>();
    std::cout << std::endl;
    std::cout << "reachable " << std::endl;
    reachable.print<[](const bool &reach) {std::cout << (reach ? 1 : 0) << " ";}>();
    std::cout << std::endl;
    std::cout << "minable frontier " << std::endl;
    minable_frontier.print<[](const bool &minable) {std::cout << (minable ? 1 : 0) << " ";}>();
  }
  
  bool Map::checkBresenham(Position start, Position end) {
    int dx = start.x - end.x;
    int dy = start.y - end.y;

    int d_long = dx;
    int d_short = dy;

    int long_axis = end.x;
    int short_axis = end.y;

    int long_mod = 1;
    int short_mod = 1;

    auto XLong = [](Kilometer long_axis, Kilometer short_axis) -> Position {
      return {long_axis, short_axis};
    };
      
    auto YLong = [](Kilometer long_axis, Kilometer short_axis) -> Position {
      return {short_axis, long_axis};
    };

    bool use_y_long_flag = false;
    // auto create_position = XLong;

    if (abs(dy) > abs(dx)) {
      d_long = dy;
      d_short = dx;
      long_axis = end.y;
      short_axis = end.x;
      use_y_long_flag = true;
      // create_position = YLong;
    }

    if (d_long < 0) {
      long_mod = -1;
    }
    if (d_short < 0) {
      short_mod = -1;
    }

    d_long = abs(d_long);
    d_short = abs(d_short);

    auto p = 2 * d_short - d_long;

    int count = 0;
    while (count < d_long) {
      if (use_y_long_flag) {
	if (!can_go(YLong((Kilometer)long_axis, (Kilometer)short_axis))) return false;
      }
      else {
	if (!can_go(XLong((Kilometer)long_axis, (Kilometer)short_axis))) return false;
      }
      if (p >= 0) {
	short_axis = short_axis + short_mod;
	p = p + 2 * (d_short - d_long);
      } else {
	p = p + 2 * d_short;
      }
      long_axis = long_axis + long_mod;
      count += 1;
    }
    return true;
  }

  void Map::proc_reachability(Position p, bool is_traversable) {
    if (is_traversable) {
      for (auto [pos, reach]: reachable.square_indexed(p, 1)) {
	if (!can_go(pos)) continue;
	if (reach) reachable[p] = true;
      }
      if (reachable[p]) propagate_reachability(p);
    }
    else {
      for (auto [pos, minable]: minable_frontier.circle_indexed(p, sim.map_config.action_mine_max_distance)) {
	if (!can_go(pos)) continue;
	if (reachable[pos]) minable = true;
      }
    }
  }

  void Map::propagate_reachability(Position p) {
    std::queue<Position> q;
    q.push(p);
    while (q.size() > 0) {
      Position pos = q.front();
      q.pop();
      for (auto [neighbor, reach] : reachable.square_indexed(pos, 1)) {
	if (!reach && can_go(neighbor)) {
	  reach = true;
	  q.push(neighbor);
	}
      }
      for (auto &minable : minable_frontier.circle(pos, sim.map_config.action_mine_max_distance)) {
	minable = true;
      }
    }
  }
}

