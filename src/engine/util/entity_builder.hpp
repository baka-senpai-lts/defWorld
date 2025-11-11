#ifndef ENTITY_BUILDER_H_
#define ENTITY_BUILDER_H_

#include "../ecs/component.hpp"
#include "../ecs/entity.hpp"

#include <memory>
#include <vector>

namespace engine::util {

class EntityBuilder {
protected:
  std::vector<std::shared_ptr<ecs::Component>> components;

public:
  template <typename T, typename... Args> EntityBuilder &with(Args &&...args) {
    components.push_back(std::make_shared<T>(std::forward<Args>(args)...));
    return *this;
  }

  std::shared_ptr<ecs::Entity> assemble();
};

} // namespace engine::util

#endif // ENTITY_BUILDER_H_
