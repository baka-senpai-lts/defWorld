#include "system_scheduler.hpp"
#include "../log/logger.hpp"

#include <vector>

namespace engine::base {

SystemScheduler::SystemScheduler() {}

void SystemScheduler::run(
    const std::unordered_map<std::string, std::shared_ptr<ecs::System>>
        &systems,
    ecs::World &world, float delta) {
  /*
   * A very naive implementation, shall be rewritten from scratch, but should
   * work for prototyping
   */

  // TODO: rework
  bool cache_hit = true;

  if (last_run_call.size() != systems.size()) {
    cache_hit = false;
    goto SKIP_CACHE_CHECK;
  }

  for (const auto &[id, system] : systems) {
    if (last_run_call.count(system) == 0) {
      cache_hit = false;
      break;
    }
  }

SKIP_CACHE_CHECK:
  if (cache_hit) {
    for (const auto &[id, system] : last_run_sorted) {
      system->run(world, delta);
    }

    return;
  }

  std::vector<std::pair<std::string, std::shared_ptr<ecs::System>>>
      systems_sorted;

  last_run_call.clear();

  // Simple insertion sort for systems based on phase and dependencies
  for (const auto &[id, system] : systems) {
    MT_LOG_DEBUG("Finding position for {}", id);

    last_run_call.insert(system);

    bool inserted = false;

    auto dependencies = system->getDependencies();
    auto pos = systems_sorted.begin();

    for (const auto &[id_sorted, system_sorted] : systems_sorted) {
      dependencies.erase(id_sorted);

      if (dependencies.empty() &&
          (system_sorted->getPhase() >= system->getPhase())) {
        systems_sorted.insert(pos, {id, system});
        inserted = true;
        break;
      }

      pos++;
    }

    if (!inserted) {
      systems_sorted.push_back({id, system});
    }
  }

  last_run_sorted = systems_sorted;
  for (const auto &[id, system] : systems_sorted) {
    MT_LOG_DEBUG("Running system {}", id);
    system->run(world, delta);
  }
}

} // namespace engine::base
