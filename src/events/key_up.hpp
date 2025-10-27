#ifndef KEY_UP_H_
#define KEY_UP_H_

#include "event.hpp"
#include <raylib.h>

namespace engine::event {

class KeyUpEvent : public Event {
private:
  int key;

public:
  KeyUpEvent() {
    type = EventType::KEY_UP;
    key = KeyboardKey::KEY_NULL;
  }

  KeyUpEvent(int key) : engine::event::KeyUpEvent() { this->key = key; }

  ~KeyUpEvent() {}

  int getKey() const { return key; }
};

} // namespace engine::event

#endif // KEY_UP_H_
