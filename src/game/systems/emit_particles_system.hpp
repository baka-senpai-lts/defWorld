#ifndef EMIT_PARTICLES_SYSTEM_H_
#define EMIT_PARTICLES_SYSTEM_H_

#include "engine/ecs/system.hpp"
#include "engine/render/texture_provider.hpp"

class EmitParticlesSystem : public engine::ecs::System {
protected:
  std::shared_ptr<engine::render::TextureProvider> texture_provider;
  uint64_t i; // will die out quickly

public:
  EmitParticlesSystem(std::shared_ptr<engine::render::TextureProvider> tp) {
    phase = engine::ecs::LOGIC;
    id = "emit-particles-system";
    texture_provider = tp;
    i = 0;
  }
  ~EmitParticlesSystem() override = default;

  void run(engine::ecs::World &world, float delta) override;
};

#endif // EMIT_PARTICLES_SYSTEM_H_
