#pragma once

#include <bits/stdc++.h>
#include "grid.h"
#include "gui/constants.h"
#include "gui/cell.h"
#include "gui/bot.h"

// Defining the map and the bot

namespace GUI {
  using miningbots::Grid;
  extern Grid<Cell> grid;
  extern SDL_Point botPosition;
  extern std::unordered_map<uint32_t, Bot> botPosition_map;
  extern SDL_Rect gCellRect;
  extern SDL_Window* gWindow;
  extern SDL_Renderer* gRenderer;
}
