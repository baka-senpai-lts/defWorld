#ifndef DRAWABLE_COMPONENT_H_
#define DRAWABLE_COMPONENT_H_

#include "engine/ecs/component.hpp"
#include "engine/render/drawable_primitives/sprite.hpp"

#include <memory>

class SpriteComponent : public engine::ecs::ComponentBase<SpriteComponent> {
public:
  SpriteComponent(std::shared_ptr<engine::render::Sprite> d) : Base() {
    sprite = d;
  }

  template <typename... Args> SpriteComponent(Args &&...args) : Base() {
    sprite =
        std::make_shared<engine::render::Sprite>(std::forward<Args>(args)...);
  }

  ~SpriteComponent() override = default;

  std::shared_ptr<engine::render::Sprite> sprite;
};

#endif // DRAWABLE_COMPONENT_H_
