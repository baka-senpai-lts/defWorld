#ifndef MOUSE_WHEEL_MOVE_H_
#define MOUSE_WHEEL_MOVE_H_

#include "event.hpp"
#include <raylib.h>

namespace game::event {

class MouseWheelMoveEvent : public Event {
private:
  float movement;

public:
  MouseWheelMoveEvent() {
    type = EventType::MOUSE_WHEEL_MOVE;
    movement = 0;
  }

  MouseWheelMoveEvent(float movement) : game::event::MouseWheelMoveEvent() {
    this->movement = movement;
  }

  ~MouseWheelMoveEvent() {}

  float getWheelMovement() const { return movement; }
};

} // namespace game::event

#endif // MOUSE_WHEEL_MOVE_H_
