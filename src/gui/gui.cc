#include "gui/bot.h"
#include "gui/variable.h"
#include "json/update.h"
#include "gui/get_updates.h"
#include "utils.h"

namespace GUI {
  
Grid<Cell> grid(NUM_CELLS_X, NUM_CELLS_Y);
std::unordered_map<uint32_t, Bot> botPosition_map;
SDL_Rect gCellRect = {0, 0, GRID_SIZE, GRID_SIZE};
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
  
// Initialising/Opening SDL window
bool initSDL()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    return false;
  }

  gWindow = SDL_CreateWindow("Mining Game", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (gWindow == nullptr)
  {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    return false;
  }

  gRenderer = SDL_CreateRenderer(
      gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (gRenderer == nullptr)
  {
    std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    return false;
  }

  return true;
}

// Closing the window
void closeSDL()
{
  SDL_DestroyRenderer(gRenderer);
  gRenderer = nullptr;

  SDL_DestroyWindow(gWindow);
  gWindow = nullptr;

  SDL_Quit();
}
}



