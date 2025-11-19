#ifndef POSITION_COMPONENT_H_
#define POSITION_COMPONENT_H_

#include <Vector2.hpp>

#include "../../engine/ecs/component.hpp"

class PositionComponent : public engine::ecs::ComponentBase<PositionComponent> {
public:
  PositionComponent() : Base() {}
  PositionComponent(Vector2 pos) : Base() { this->pos = pos; }
  ~PositionComponent() override = default;

  Vector2 pos;
};

#endif // DRAWABLE_COMPONENT_H_
