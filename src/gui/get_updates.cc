#include "gui/get_updates.h"
#include "glaze/glaze.hpp"
#include <iostream>

namespace GUI {
// Handling single update
void single_update(miningbots::json::Update single_update)
{

    
    // handling botupdate
    std::vector<miningbots::json::BotUpdate> &botupdates = single_update.bot_updates;
    for (const auto &botupdate : botupdates)
    {
        BotId botid = botupdate.id;
        botPosition_map[botid].position.x = botupdate.position.x;
        botPosition_map[botid].position.y = botupdate.position.y;
        botPosition_map[botid].variant = botupdate.variant;
    }

    // handling landupdate (mainly for scaning)
    std::vector<miningbots::json::LandUpdate> &landupdates = single_update.land_updates;
    for (const auto &landupdate : landupdates)
    {
        if (landupdate.is_traversable)
        {
            if (landupdate.resources.size() != 0)
                grid[landupdate.position].type = CellType::Resource;
            else
                grid[landupdate.position].type = CellType::Empty;
        }
        else
        {
            grid[landupdate.position].type = CellType::Obstacle;
        }
        grid[landupdate.position].scanned = true;
    }

    return;
}

// Parsing each update
void parsing_the_updates(std::vector<miningbots::json::Update> all_updates)
{
    for (auto &update : all_updates)
    {
        single_update(update);
        drawGrid(botPosition_map);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(TIME_DELAY);
    }
}
}
