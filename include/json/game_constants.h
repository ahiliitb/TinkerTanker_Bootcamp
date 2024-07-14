#pragma once

#include <string>

namespace miningbots::json {
  
  struct GameConstants {
    unsigned int scan_refresh_threshold{3};
    std::string host_name{"server.bootcamp.tk.sg"};
    int port{9002};
    bool restart_at_begining{true};
    bool show_gui{false};
    bool store_updates{true};
    std::string updates_output_file{"updatelist.txt"};
    unsigned int seconds_to_run_for{100};
    unsigned int mobility_reduction_factor{2};
    unsigned int scan_bots_fraction{3};
    unsigned int max_move_dist{5};
  };
  
}
