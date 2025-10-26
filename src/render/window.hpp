#ifndef WINDOW_H_
#define WINDOW_H_

#include "../events/event.hpp"
#include "drawable.hpp"
#include "raylib-cpp.hpp" // IWYU pragma: keep

#include <cstdint>
#include <memory>
#include <queue>
#include <raylib.h>
#include <string>
#include <unordered_map>

namespace game::render {

// Only used in here, later in stack you will only see events
enum class KeyState { Up, Down };

class Window {
private:
  uint32_t size_x;
  uint32_t size_y;
  uint32_t target_fps;
  std::string window_name;
  std::queue<std::shared_ptr<Event>> event_queue;

  // Exists to not push infinite amount of KeyDownEvents
  std::unordered_map<KeyboardKey, KeyState> keyboard_state;
  std::unordered_map<MouseButton, KeyState> mouse_state;

  Vector2 mouse_pos;

  std::unique_ptr<raylib::Window> window;

public:
  Window(uint32_t size_x, uint32_t size_y, std::string window_name);
  Window(uint32_t size_x, uint32_t size_y, std::string window_name,
         uint32_t target_fps);
  ~Window() {};

  void pollEvents();

  bool shouldClose() const;
  float getFrameTime() const;

  size_t getEventQueueSize() const;
  std::shared_ptr<Event> popEvent();

  void init();
  void drawFrame(const std::vector<std::shared_ptr<Drawable>> &drawables);
  void die();
};

} // namespace game::render

#endif // WINDOW_H_
