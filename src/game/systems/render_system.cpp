#include "render_system.hpp"

#include "../components/sprite_component.hpp"

void RenderSystem::run(engine::ecs::World &world, float delta) {
  auto entities = world.query<SpriteComponent>();

  std::vector<std::shared_ptr<engine::render::Drawable>> drawables;

  for (const auto &[entity, _] : entities) {
    drawables.push_back(entity->get<SpriteComponent>()->sprite);
  }

  window->drawFrame(drawables);
}
