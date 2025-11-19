#include "drawable_position_system.hpp"

#include "../components/position_component.hpp"
#include "../components/sprite_component.hpp"

void DrawablePositionSystem::run(engine::ecs::World &world, float delta) {
  auto entities = world.query<SpriteComponent, PositionComponent>();

  for (const auto &[entity, _] : entities) {
    const Vector2 pos = entity->get<PositionComponent>()->pos;

    entity->get<SpriteComponent>()->sprite->setPos(pos);
  }
}
