#include "entity_builder.hpp"
#include "engine/ecs/entity.hpp"

namespace engine::util {

std::shared_ptr<ecs::Entity> EntityBuilder::assemble() {
  auto entity = std::make_shared<ecs::Entity>();

  for (auto &component : components) {
    entity->addComponent(std::move(component));
  }
  components.clear();

  return std::move(entity);
}

} // namespace engine::util
