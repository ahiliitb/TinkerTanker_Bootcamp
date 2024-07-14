#include "gui/map.h"

namespace GUI {
// Creating the map
void drawGrid(std::unordered_map<BotId, Bot> botPosition_map)
{
  for (Kilometer i = 0; i < NUM_CELLS_X; ++i)
  {
    for (Kilometer j = 0; j < NUM_CELLS_Y; ++j)
    {
      gCellRect.x = i * GRID_SIZE;
      gCellRect.y = j * GRID_SIZE;

      switch (grid[{i, j}].type)
      {
      case CellType::Empty:
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255,
                               255); // White for empty cell
        break;
      case CellType::Obstacle:
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); // Red for obstacle
        break;
      case CellType::Resource:
        SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255); // Green for resource
        break;
      default:
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255); // Blue for unknown
        break;
      }
      if (!grid[{i, j}].scanned)
      {
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0,
                               255); // Black border for unscanned cells
      }
      SDL_RenderFillRect(gRenderer, &gCellRect);
    }
  }

  // Draw all the bots
  for (const auto &botpos : botPosition_map)
  {
    gCellRect.x = botpos.second.position.x * GRID_SIZE;
    gCellRect.y = botpos.second.position.y * GRID_SIZE;

    if(botpos.second.variant == miningbots::json::Variant::kMiningBot)
    {
      SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255); // Yellow for mining bot
    }
    else SDL_SetRenderDrawColor(gRenderer, 255, 155, 0, 255); // for buildbot bot
    SDL_RenderFillRect(gRenderer, &gCellRect);
  }
}
}
