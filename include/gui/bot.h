#pragma once

#include "base_types.h"
#include <SDL2/SDL.h>
#include "json/bot_update.h"

namespace GUI {
  // defining the bot
  struct Bot {
    BotId id;
    SDL_Point position;
    miningbots::json::Variant variant;
  };
}
