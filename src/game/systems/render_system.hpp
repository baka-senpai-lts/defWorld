#ifndef RENDER_SYSTEM_H_
#define RENDER_SYSTEM_H_

#include "engine/ecs/system.hpp"
#include "engine/render/window.hpp"

class RenderSystem : public engine::ecs::System {
protected:
  std::shared_ptr<engine::render::Window> window;

public:
  RenderSystem(std::shared_ptr<engine::render::Window> win) {
    phase = engine::ecs::RENDER;
    id = "render-system";
    window = win;
  }
  ~RenderSystem() override = default;

  void run(engine::ecs::World &world, float delta) override;
};

#endif // RENDER_SYSTEM_H_
