#pragma once

#include "../base_types.h"
#include "json/resource_chunk.h"
#include "json/resource_config.h"
#include "json/terrain_config.h"

namespace miningbots::json {

struct ExtraConfig {
  bool is_competition_server{false};
  bool fix_traversable_bug{false};
  int min_clusters{6};
  int max_clusters{10};
  int min_cluster_height{2};
  int max_cluster_height{4};
  int min_cluster_width{3};
  int max_cluster_width{6};
  int max_resource_per_chunk{2000};
  int min_resource_per_chunk{200};
};
} // namespace miningbots::json