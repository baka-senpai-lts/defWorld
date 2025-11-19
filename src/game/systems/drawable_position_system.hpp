#ifndef DRAWABLE_POSITION_SYSTEM_H_
#define DRAWABLE_POSITION_SYSTEM_H_

#include "engine/ecs/system.hpp"

class DrawablePositionSystem : public engine::ecs::System {
public:
  DrawablePositionSystem() {
    phase = engine::ecs::EARLY;
    dependencies = {"follow-cursor-system"};
    id = "drawable-position-system";
  }
  ~DrawablePositionSystem() override = default;

  void run(engine::ecs::World &world, float delta) override;
};

#endif // DRAWABLE_POSITION_SYSTEM_H_
