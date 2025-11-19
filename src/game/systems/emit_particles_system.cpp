#include "emit_particles_system.hpp"

#include "engine/util/entity_builder.hpp"

#include "../components/emit_particles_component.hpp"
#include "../components/particle_component.hpp"
#include "../components/position_component.hpp"
#include "../components/sprite_component.hpp"

void EmitParticlesSystem::run(engine::ecs::World &world, float delta) {
  auto entities = world.query<EmitParticlesComponent, PositionComponent>();

  for (const auto &[entity, _] : entities) {
    auto emit_particles = entity->get<EmitParticlesComponent>();

    emit_particles->current =
        std::max(emit_particles->current - delta * emit_particles->rate, 0.f);

    if (1 <= emit_particles->current) {
      continue;
    }

    emit_particles->current += 1;

    Vector2 position = entity->get<PositionComponent>()->pos;

    std::string particle_id = fmt::format("particle-{}", i);

    world.addEntity(particle_id,
                    engine::util::EntityBuilder()
                        .with<ParticleComponent>(

#ifdef _WIN32
                            Vector2{(float)(rand() % 600 - 300),
                                    (float)(rand() % 600 - 300)},
#else
                            Vector2{(float)(random() % 600 - 300),
                                    (float)(random() % 600 - 300)},
#endif
                            1.f)
                        .with<PositionComponent>(position)
                        .with<SpriteComponent>(
                            texture_provider->load("sample-sprite"), position)
                        .assemble());
    i++;
  }
}
