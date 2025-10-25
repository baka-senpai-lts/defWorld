#ifndef MOUSE_DOWN_H_
#define MOUSE_DOWN_H_

#include "event.hpp"
#include <raylib.h>

namespace game::event {

class MouseDownEvent : public Event {
private:
  int button;

public:
  MouseDownEvent() {
    type = EventType::MOUSE_DOWN;

    // MouseButton enum doesn't contain a convenient placeholder for no button
    button = 0;
  }

  MouseDownEvent(int button) : game::event::MouseDownEvent() {
    this->button = button;
  }

  ~MouseDownEvent() {}

  int getButton() const { return button; }
};

} // namespace game::event

#endif // MOUSE_DOWN_H_
