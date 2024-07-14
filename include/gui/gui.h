#pragma once

#include "gui/bot.h"
#include "gui/variable.h"
#include "gui/get_updates.h"

namespace GUI{
  bool initSDL();
  void closeSDL();
  void single_update(miningbots::json::Update single_update);
}
