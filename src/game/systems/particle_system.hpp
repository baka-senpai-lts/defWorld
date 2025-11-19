#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include "engine/ecs/system.hpp"

class ParticleSystem : public engine::ecs::System {
public:
  ParticleSystem() {
    phase = engine::ecs::PHYSICS;
    dependencies = {"emit-particles-system"};
    id = "particle-system";
  }
  ~ParticleSystem() override = default;

  void run(engine::ecs::World &world, float delta) override;
};

#endif // PARTICLE_SYSTEM_H_
