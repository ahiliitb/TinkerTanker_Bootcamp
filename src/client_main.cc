#include "player.h"
#include <random>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include "json/error_response.h"
#include "utils.h"
#include "sim.h"


int main(int, char **) {

  // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
  spdlog::set_pattern("[%D %H:%M:%S.%F] [%^%l%$] [%s %!:%#] [%oms] [%ius]  %v");
  spdlog::set_level(spdlog::level::debug);
  // spdlog::cfg::load_env_levels();
  
  const std::string config_path = "../config/";
  
  miningbots::Sim gsim(config_path);

  ix::OnMessageCallback cb = [&gsim] (const ix::WebSocketMessagePtr &message) {
    miningbots::Sim::messageCallback(gsim, message);
  };

  gsim.addPlayer("Ramanuj2", 4, cb);

  gsim.run();
  
}
