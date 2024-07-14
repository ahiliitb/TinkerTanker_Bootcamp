#include "scan_manager.h"
#include "map.h"
#include "sim.h"
#include "bot.h"
#include <iostream>

namespace miningbots {

  
  ScanManager::ScanManager(const Sim &s):
    sim(s),
    unexplored_in_scan_radius(s.max_x(), s.max_y(), 0),
    unexplored_in_scan_distance(s.max_x(), s.max_y(), 0),
    has_sent_scan_request(s.max_x(), s.max_y(), 0)
  {
    init_unexplored_in_scan_distance();
    init_unexplored_in_scan_radius();
  }

  Position ScanManager::findUnexploredNearby(const Position &p, Bot &bot) {
    Position best = p;
    double curmax = 0;
    double temp;
    for (auto [pos, unexplored_value] : unexplored_in_scan_distance.cells_indexed()) {
      // only look for traversable locations
      if (!bot.can_reach(pos)) continue;
      temp = unexplored_value;
      temp /= (1 + bot.distance_to(pos));
      if (temp > curmax) {
	curmax = temp;
	best = pos;
      }
    }
    return best;
  }
  
  std::optional<Position> ScanManager::findUnexplored_scan(const Position &p, Map &map) {
    std::optional<Position> best = std::nullopt;
    unsigned int curmax = 0;
    // std::cout << "my position: " << p.x << " " << p.y << std::endl;
    for (auto [pos, unexplored_value]: unexplored_in_scan_radius.circle_indexed(p, sim.map_config.action_scan_max_distance)) {
      // std::cout << "Position " << pos.x << " " << pos.y << std::endl;
      if (map.is_scanned(pos)) {
	// std::cout << "rejecting position: is scanned " << std::endl;
	continue;
      }
      if (unexplored_value > curmax) {
	curmax = unexplored_value;
	best = std::optional<Position>(pos);
      }
      else {
	// std::cout << "rejecting position: not best" << std::endl;
      }
    }
    return best;
  }

  void ScanManager::init_unexplored_in_scan_distance() {
    // inefficient solution for now
    for (auto [pos, unexplore_val] : unexplored_in_scan_distance.cells_indexed()) {
      for ([[maybe_unused]] auto unused : unexplored_in_scan_distance.circle(pos, sim.map_config.action_scan_max_distance)) {
	unexplore_val++;
      }
    }
  }

  // :( cannot implement this efficiently with the range stuff
  // std::ranges::size doesn't work :(
  void ScanManager::init_unexplored_in_scan_radius() {
    int rows = sim.max_x();
    int cols = sim.max_y();
    int scan_radius = sim.map_config.action_scan_radius;
    for (auto [pos, unexplored]: unexplored_in_scan_radius.cells_indexed()) {
      int Xmin = std::max((int)pos.x - scan_radius, 0);
      int Xmax = std::min((int)pos.x + scan_radius, rows-1);
      int Ymin = std::max((int)pos.y - scan_radius, 0);
      int Ymax = std::min((int)pos.y + scan_radius, cols-1);
      unexplored = (Xmax - Xmin + 1) * (Ymax - Ymin + 1);
    }
  }

  void ScanManager::update_unexplored_in_scan_distance(Position p, bool is_positive) {
    for (auto [pos, unexplore_val]: unexplored_in_scan_distance.circle_indexed(p, sim.map_config.action_scan_max_distance)) {
      if (is_positive) unexplore_val ++;
      else unexplore_val --;
    }
  }

  void ScanManager::update_unexplored_in_scan_radius(Position p, bool is_positive) {
    for (auto [pos, unexplore_val]: unexplored_in_scan_radius.square_indexed(p, sim.map_config.action_scan_radius)) {
      if (is_positive) unexplore_val ++;
      else unexplore_val --;
    }
  }

  void ScanManager::refresh_from_map(Map &map, unsigned int iter) {
    last_iteration = iter;
    for (auto [pos, traverse]: map.is_traversable.cells_indexed()) {
      if (has_sent_scan_request[pos] > 0 &&
	  traverse == Map::Traversable::Unknown &&
	  iter - has_sent_scan_request[pos] >
	  sim.game_constants.scan_refresh_threshold) {
	has_sent_scan_request[pos] = 0;
	num_scanned --;
	update_unexplored_in_scan_radius(pos, true);
	update_unexplored_in_scan_distance(pos, true);
      }
      else if (has_sent_scan_request[pos] == 0 &&
	       traverse != Map::Traversable::Unknown) {
	notify_future_scan(pos);
      }
    }
  }

  double ScanManager::get_scan_percentage() {
    return ((double)num_scanned) / ((double)sim.max_x() * sim.max_y());
  }

  void ScanManager::print() {    
    std::cout << "scan requested " << std::endl;
    has_sent_scan_request.print<[](const bool &x) { std::cout << (x ? 1 : 0) << " "; }>();
    std::cout << std::endl;
    std::cout << "unexplored in scan distance " << std::endl;
    unexplored_in_scan_distance.print<[](const unsigned int &x) { std::cout << x << " "; }>();
    std::cout << std::endl;
    std::cout << "unexplored in scan radius " << std::endl;
    unexplored_in_scan_radius.print<[](const unsigned int &x) { std::cout << x << " "; }>();
    std::cout << std::endl;
  }
  
  void ScanManager::notify_future_scan(Position p) {
    if (has_sent_scan_request[p]) return;
    has_sent_scan_request[p] = last_iteration;
    num_scanned ++;
    update_unexplored_in_scan_radius(p, false);
    update_unexplored_in_scan_distance(p, false);
  }
  
  // TODO: make this more efficient
  void ScanManager::notify_sent_scan_request(Position p) {
    for ([[maybe_unused]] auto [pos, unused] : has_sent_scan_request.square_indexed(p, sim.map_config.action_scan_radius)) {
      notify_future_scan(pos);
    }
  }
  
}
