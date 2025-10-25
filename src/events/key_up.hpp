#ifndef KEY_UP_H_
#define KEY_UP_H_

#include "event.hpp"
#include <raylib.h>

namespace game::event {

class KeyUpEvent : public Event {
private:
  int key;

public:
  KeyUpEvent() {
    type = EventType::KEY_UP;
    key = KeyboardKey::KEY_NULL;
  }

  KeyUpEvent(int key) : game::event::KeyUpEvent() { this->key = key; }

  ~KeyUpEvent() {}

  int getKey() const { return key; }
};

} // namespace game::event

#endif // KEY_UP_H_
