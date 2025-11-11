#ifndef SYSTEM_SCHEDULER_H_
#define SYSTEM_SCHEDULER_H_

#include "../ecs/system.hpp"
#include "../ecs/world.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace engine::base {

class SystemScheduler {
private:
  std::unordered_set<std::shared_ptr<ecs::System>> last_run_call;
  std::vector<std::pair<std::string, std::shared_ptr<ecs::System>>>
      last_run_sorted;

public:
  SystemScheduler();
  ~SystemScheduler() = default;

  void run(const std::unordered_map<std::string, std::shared_ptr<ecs::System>>
               &systems,
           ecs::World &world, float delta);
};

} // namespace engine::base

#endif // SYSTEM_SCHEDULER_H_
