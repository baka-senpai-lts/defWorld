#ifndef KEY_DOWN_H_
#define KEY_DOWN_H_

#include "event.hpp"
#include <raylib.h>

namespace engine::event {

class KeyDownEvent : public Event {
private:
  int key;

public:
  KeyDownEvent() {
    type = EventType::KEY_DOWN;
    key = KeyboardKey::KEY_NULL;
  }

  KeyDownEvent(int key) : engine::event::KeyDownEvent() { this->key = key; }

  ~KeyDownEvent() {}

  int getKey() const { return key; }
};

} // namespace engine::event

#endif // KEY_DOWN_H_
