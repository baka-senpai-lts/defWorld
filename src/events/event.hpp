#ifndef EVENT_H_
#define EVENT_H_

namespace game {

enum EventType { NONE, KEY_UP, KEY_DOWN, MOUSE_MOVE, MOUSE_UP, MOUSE_DOWN };

class Event {
protected:
  EventType type;

public:
  Event() : type(EventType::NONE) {}
  virtual ~Event();

  EventType getEventType() const { return type; };
};

} // namespace game

#endif // EVENT_H_
