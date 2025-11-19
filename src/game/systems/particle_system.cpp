#include "particle_system.hpp"

#include "../components/particle_component.hpp"
#include "../components/position_component.hpp"
#include "../components/sprite_component.hpp"

void ParticleSystem::run(engine::ecs::World &world, float delta) {
  auto entities =
      world.query<SpriteComponent, ParticleComponent, PositionComponent>();

  for (const auto &[entity, id] : entities) {
    auto particle = entity->get<ParticleComponent>();

    particle->lifetime += delta;

    if (particle->lifetime >= particle->max_lifetime) {
      world.removeEntity(id);
    }

    particle->vel.y += 1000 * delta;

    auto position = entity->get<PositionComponent>();

    position->pos.x += particle->vel.x * delta;
    position->pos.y += particle->vel.y * delta;

    entity->get<SpriteComponent>()->sprite->setScale(
        1 - (particle->lifetime / particle->max_lifetime));
  }
}
