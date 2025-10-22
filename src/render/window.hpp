#ifndef WINDOW_H_
#define WINDOW_H_

#include "../events/event.hpp"
#include "Window.hpp"

#include <cstdint>
#include <memory>
#include <queue>
#include <string>

namespace game::render {

class Window {
private:
  uint32_t size_x;
  uint32_t size_y;
  uint32_t target_fps;
  std::string window_name;
  std::queue<std::unique_ptr<Event>> event_queue;

  std::unique_ptr<raylib::Window> window;

public:
  Window(uint32_t size_x, uint32_t size_y, std::string window_name);
  Window(uint32_t size_x, uint32_t size_y, std::string window_name,
         uint32_t target_fps);
  ~Window() {};

  bool shouldClose() const;
  float getFrameTime() const;

  size_t getEventQueueSize() const;
  std::unique_ptr<Event> popEvent();

  void init();
  void drawFrame();
  void die();
};

} // namespace game::render

#endif // WINDOW_H_
