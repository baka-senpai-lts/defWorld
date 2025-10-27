#ifndef EVENT_H_
#define EVENT_H_

namespace engine {

enum EventType {
  NONE,
  KEY_UP,
  KEY_DOWN,
  MOUSE_MOVE,
  MOUSE_UP,
  MOUSE_DOWN,
  MOUSE_WHEEL_MOVE,
  WINDOW_RESIZE
};

class Event {
protected:
  EventType type;

public:
  Event() : type(EventType::NONE) {}
  ~Event() {}

  EventType getEventType() const { return type; };
};

} // namespace engine

#endif // EVENT_H_
