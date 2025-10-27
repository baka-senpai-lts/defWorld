#ifndef MOUSE_MOVE_H_
#define MOUSE_MOVE_H_

#include "event.hpp"
#include <raylib.h>

namespace engine::event {

class MouseMoveEvent : public Event {
private:
  Vector2 pos;

public:
  MouseMoveEvent() {
    type = EventType::MOUSE_MOVE;
    pos.x = 0;
    pos.y = 0;
  }

  MouseMoveEvent(Vector2 pos) : engine::event::MouseMoveEvent() {
    this->pos.x = pos.x;
    this->pos.y = pos.y;
  }

  ~MouseMoveEvent() {}

  Vector2 getPos() const { return this->pos; }
  int getX() const { return pos.x; }
  int getY() const { return pos.y; }
};

} // namespace engine::event

#endif // MOUSE_MOVE_H_
