#ifndef EMIT_PARTICLES_COMPONENT_H_
#define EMIT_PARTICLES_COMPONENT_H_

#include "../../engine/ecs/component.hpp"

class EmitParticlesComponent
    : public engine::ecs::ComponentBase<EmitParticlesComponent> {
public:
  ~EmitParticlesComponent() override = default;

  float rate = 100;
  float current = 0;
};

#endif // EMIT_PARTICLES_COMPONENT_H_
