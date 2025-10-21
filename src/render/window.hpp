#ifndef WINDOW_H_
#define WINDOW_H_

#include "Window.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace game::render {

class Window {
private:
  uint32_t size_x;
  uint32_t size_y;
  uint32_t target_fps;
  std::string window_name;

  std::unique_ptr<raylib::Window> window;

public:
  Window(uint32_t size_x, uint32_t size_y, std::string window_name);
  Window(uint32_t size_x, uint32_t size_y, std::string window_name,
         uint32_t target_fps);
  ~Window() {};

  bool shouldClose();
  float getFrameTime();

  void init();
  void runFrame(float frameTime);
  void die();
};

} // namespace game::render

#endif // WINDOW_H_
