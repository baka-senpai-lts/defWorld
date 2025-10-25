#ifndef MOUSE_UP_H_
#define MOUSE_UP_H_

#include "event.hpp"
#include <raylib.h>

namespace game::event {

class MouseUpEvent : public Event {
private:
  int button;

public:
  MouseUpEvent() {
    type = EventType::MOUSE_UP;

    // MouseButton enum doesn't contain a convenient placeholder for no button
    button = 0;
  }

  MouseUpEvent(int button) : game::event::MouseUpEvent() {
    this->button = button;
  }

  ~MouseUpEvent() {}

  int getButton() const { return button; }
};

} // namespace game::event

#endif // MOUSE_UP_H_
