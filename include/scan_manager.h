#pragma once

#include "base_types.h"
#include "json/land_update.h"
#include "json/position.h"
#include <optional>
#include "grid.h"

namespace miningbots {
}

namespace miningbots {

  struct Map;
  struct Sim;
  struct Bot;
  
  using Position = json::Position;

  struct ScanManager {
  public:
    
    ScanManager(const Sim &s);
    void notify_sent_scan_request(Position p);
    void refresh_from_map(Map &map, unsigned int iter);
    Position findUnexploredNearby(const Position &p, Bot &bot);
    std::optional<Position> findUnexplored_scan(const Position &p, Map &map);
    void print();

    unsigned int num_scanned = 0;

    double get_scan_percentage();

  private:

    const Sim &sim;

    // call these two functions in the constructor only, then we can update things when we send more and more scan requests
    void init_unexplored_in_scan_distance(); // calculates how much more region i can explore if a bot goes to cell at (x, y). The region is a scan_distance circle
    void init_unexplored_in_scan_radius(); // calculates how much more region will be explored once I scan some cell at (x, y). This is a box of scan_radius by scan_radius

    void update_unexplored_in_scan_distance(Position p, bool is_positive); // calculates how much more region i can explore if a bot goes to cell at (x, y). The region is a scan_distance circle
    void update_unexplored_in_scan_radius(Position p, bool is_positive); // calculates how much more region will be explored once I scan some cell at (x, y). This is a box of scan_radius by scan_radius

    void notify_future_scan(Position p);

    unsigned int last_iteration = 0;
    
    Grid<unsigned int> unexplored_in_scan_radius; // unexplored_in_scan_radius[x][y] tells us the number of un-scanned cells in a box of size scan_radius x scan_radius around x, y
    Grid<unsigned int> unexplored_in_scan_distance; // unexplored_in_scan_radius[x][y] tells us the number of un-scanned cells in a circle of radius scan_distance around x, y
    Grid<unsigned int> has_sent_scan_request; // a map of if scan request has been sent for this location
  };

}
