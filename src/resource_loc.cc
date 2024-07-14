#include "resource_loc.h"
#include "map.h"
#include "player.h"
#include "resource_manager.h"

namespace miningbots {

  void ResourceLoc::calc_value_time_and_energy(ResourceManager &manager, int n_bots) {
    manager.sort_chunks_by_value(chunks);
    base_value_sum = manager.calculate_chunks_value(chunks);
    base_time_sum = manager.calculate_chunks_time(chunks);
    base_energy_sum = manager.calculate_chunks_energy(chunks);
    bot_value_time_ratio_and_index.resize(n_bots);
  }

  void ResourceLoc::add_bots(const std::vector<Bot> &bots, ResourceManager &manager, Position pos) {
    int bot_idx = 0;
    for (const Bot &bot: bots) {
      if (bot.is_minining_bot()) {
	double val = manager.calculate_value_for_bot_and_resource(pos, *this, bot);
	bot_value_time_ratio_and_index[bot_idx] = {val, bot_idx};
      }
      else {
	bot_value_time_ratio_and_index[bot_idx] = {-1, bot_idx};
      }
      bot_idx ++;
    }
    sort(bot_value_time_ratio_and_index.begin(), bot_value_time_ratio_and_index.end(), [](const std::pair<double, int> &a, const std::pair<double, int> &b) { return a.first > b.first; });
  }

  std::optional<std::pair<double, int>> ResourceLoc::get_bot_to_assign(std::vector<bool> &bots_taken, double threshold) {
    for (auto [value, bot_idx]: bot_value_time_ratio_and_index) {
      if (value <= threshold) return std::nullopt;
      else if (bots_taken[bot_idx] == false) {
	bots_taken[bot_idx] = true;
	return std::optional<std::pair<double, int>>({value, bot_idx});
      }
    }
    return std::nullopt;
  }
  
}
