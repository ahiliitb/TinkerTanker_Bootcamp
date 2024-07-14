#pragma once

#include "base_types.h"
#include "json/terrain.h"
#include "json/resource_chunk.h"
#include "json/land_update.h"
#include "json/bot_update.h"
#include "queue_bfs.h"
#include "meet_in_the_middle.h"
#include "resource_loc.h"
#include "scan_manager.h"
#include <iostream>
#include "grid.h"
#include "sim.h"

namespace miningbots {
  
  using Terrain = json::Terrain;
  using ResourceChunk = json::ResourceChunk;
  struct ScanManager;
  
  struct Map {

    enum class Traversable {
      No,
      Yes,
      Unknown
    };

    Map(const Sim &s);
    void procLandUpdate(const json::LandUpdate &land_update);
    void procBotUpdate(const json::BotUpdate &bot_update);
    // fill the path in path using bfs / A* etc
    template<typename R1, typename R2>
    void findPath(R1 srcs, R2 dests, std::vector<Position> &path) {
      auto can_go = [&](const Position &pos) {
	return is_traversable[pos] == Traversable::Yes;
      };
      mitm_bfs.run(srcs, dests, path, can_go);
    }
    
    // check if bresenham between start and end has any obstacles
    bool checkBresenham(Position start, Position end);
    size_t max_x() const { return is_traversable.max_x(); }
    size_t max_y() const { return is_traversable.max_y(); }
    bool can_go(Position p) const {
      return is_traversable[p] == Traversable::Yes;
    }
    bool is_scanned(const Position &p) { return is_traversable[p] != Traversable::Unknown; }

    TerrainId get_terrain_at_pos(const Position &p) { return terrain[p]; }

    void print();

    bool is_reachable(Position p) const { return reachable[p]; }
    bool is_minable(Position p) const { return reachable[p] || minable_frontier[p]; }
    
    Kilometer distance_aprox(Position a, Position b) const {
      return std::max(abs(a.x - b.x), abs(a.y - b.y));
    }
    auto get_reachable_in_circle_for_loc(Position p, unsigned int radius) {
      return filterPositionByValue<>(reachable.circle_indexed(p, radius));
    }
    
  private:

    const Sim &sim;
    
    void propagate_reachability(Position p);
    void proc_reachability(Position p, bool is_traversable);
    
    Grid<bool> reachable; // boolean matrix of if a position is reachable
    // gets updated on land-requests

    // TODO: fix this so that it is withing mine-max-distance of a reachable position
    Grid<bool> minable_frontier; // boolean matrix of if a position is adjacent to a reachable position
    // gets updated on land-requests
    
    Grid<Traversable> is_traversable;
    Grid<TerrainId> terrain;
    MeetInTheMiddle<QueueBFS> mitm_bfs;
    friend class ScanManager;
  };  
}

#include "meet_in_the_middleINLINES.cc"
