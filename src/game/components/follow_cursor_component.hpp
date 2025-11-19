#ifndef FOLLOW_CURSOR_COMPONENT_H_
#define FOLLOW_CURSOR_COMPONENT_H_

#include "../../engine/ecs/component.hpp"

class FollowCursorComponent
    : public engine::ecs::ComponentBase<FollowCursorComponent> {
public:
  ~FollowCursorComponent() override = default;
};

#endif // FOLLOW_CURSOR_COMPONENT_H_
