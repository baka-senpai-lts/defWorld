#include "window.hpp"
#include "Functions.hpp"
#include <cstdint>
#include <memory>

namespace game::render {

Window::Window(uint32_t size_x, uint32_t size_y, std::string window_name)
    : size_x(size_x), size_y(size_y), window_name(window_name), target_fps(60) {
}

Window::Window(uint32_t size_x, uint32_t size_y, std::string window_name,
               uint32_t target_fps)
    : Window::Window(size_x, size_y, window_name) {
  this->target_fps = target_fps;
}

size_t Window::getEventQueueSize() const { return event_queue.size(); }

std::unique_ptr<Event> Window::popEvent() {
  std::unique_ptr<Event> val = std::move(event_queue.front());
  event_queue.pop();
  return val;
};

void Window::init() {
  window = std::make_unique<raylib::Window>(this->size_x, this->size_y,
                                            this->window_name);
  window->SetTargetFPS(this->target_fps);
}

void Window::die() { window.reset(); }

bool Window::shouldClose() const { return window->ShouldClose(); }

float Window::getFrameTime() const { return window->GetFrameTime(); }

void Window::drawFrame() {
  window->BeginDrawing();
  window->ClearBackground(RAYWHITE);
  raylib::DrawText("Hello Raylib-cpp!", 200, 280, 20, BLACK);
  window->EndDrawing();
}

} // namespace game::render
