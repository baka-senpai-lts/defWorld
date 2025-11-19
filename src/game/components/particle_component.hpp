#ifndef PARTICLE_COMPONENT_H_
#define PARTICLE_COMPONENT_H_

#include <Vector2.hpp>

#include "engine/ecs/component.hpp"

class ParticleComponent : public engine::ecs::ComponentBase<ParticleComponent> {
public:
  ParticleComponent(Vector2 vel, float max_lifetime) : Base() {
    this->vel = vel;
    this->lifetime = 0;
    this->max_lifetime = max_lifetime;
  }
  ~ParticleComponent() override = default;

  Vector2 vel;
  float lifetime;
  float max_lifetime;
};

#endif // PARTICLE_COMPONENT_H_
