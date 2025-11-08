#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "world.hpp"

#include <string>
#include <unordered_set>

namespace engine::ecs {

enum SystemPhase {
  INPUT = 0,
  EARLY = 1,
  LOGIC = 2,
  LATE = 3,
  PHYSICS = 4,
  RENDER = 5
};

// System can store data inside of it when really needed
class System {
protected:
  std::unordered_set<std::string> dependencies;
  SystemPhase phase;
  std::string id;

public:
  System() : dependencies({}), id("replace-me"), phase(SystemPhase::LOGIC) {}
  virtual ~System() = default;

  inline const std::unordered_set<std::string> &getDependencies() const {
    return dependencies;
  }

  inline bool dependsOn(const std::string &id) const {
    return dependencies.count(id);
  }

  virtual void run(World &world, float delta) = 0;
};

} // namespace engine::ecs

#endif // SYSTEM_H_
