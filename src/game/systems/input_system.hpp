#ifndef INPUT_SYSTEM_H_
#define INPUT_SYSTEM_H_

#include "engine/ecs/system.hpp"
#include "engine/render/window.hpp"

class InputSystem : public engine::ecs::System {
protected:
  std::shared_ptr<engine::render::Window> window;

public:
  InputSystem(std::shared_ptr<engine::render::Window> win) {
    phase = engine::ecs::INPUT;
    id = "input-system";
    window = win;
  }
  ~InputSystem() override = default;

  void run(engine::ecs::World &world, float delta) override;
};

#endif // INPUT_SYSTEM_H_
