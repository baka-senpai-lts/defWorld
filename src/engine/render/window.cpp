#include "window.hpp"
#include "../events/key_down.hpp"
#include "../events/key_up.hpp"
#include "../events/mouse_down.hpp"
#include "../events/mouse_move.hpp"
#include "../events/mouse_up.hpp"
#include "../events/mouse_wheel_move.hpp"
#include "drawable.hpp"

#include "../log/logger.hpp"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <raylib.h>

namespace engine::render {

Window::Window(uint32_t size_x, uint32_t size_y, std::string window_name)
    : size_x(size_x), size_y(size_y), window_name(window_name), target_fps(60) {

  // Time to populate fucking maps with all possible keys
  // I know that this isn't optimal, but it's easier
  const size_t kb_len = 110; // Don't even ask

  KeyboardKey all_kb_keys[] = {KeyboardKey::KEY_NULL,
                               KeyboardKey::KEY_APOSTROPHE,
                               KeyboardKey::KEY_COMMA,
                               KeyboardKey::KEY_MINUS,
                               KeyboardKey::KEY_PERIOD,
                               KeyboardKey::KEY_SLASH,
                               KeyboardKey::KEY_ZERO,
                               KeyboardKey::KEY_ONE,
                               KeyboardKey::KEY_TWO,
                               KeyboardKey::KEY_THREE,
                               KeyboardKey::KEY_FOUR,
                               KeyboardKey::KEY_FIVE,
                               KeyboardKey::KEY_SIX,
                               KeyboardKey::KEY_SEVEN,
                               KeyboardKey::KEY_EIGHT,
                               KeyboardKey::KEY_NINE,
                               KeyboardKey::KEY_SEMICOLON,
                               KeyboardKey::KEY_EQUAL,
                               KeyboardKey::KEY_A,
                               KeyboardKey::KEY_B,
                               KeyboardKey::KEY_C,
                               KeyboardKey::KEY_D,
                               KeyboardKey::KEY_E,
                               KeyboardKey::KEY_F,
                               KeyboardKey::KEY_G,
                               KeyboardKey::KEY_H,
                               KeyboardKey::KEY_I,
                               KeyboardKey::KEY_J,
                               KeyboardKey::KEY_K,
                               KeyboardKey::KEY_L,
                               KeyboardKey::KEY_M,
                               KeyboardKey::KEY_N,
                               KeyboardKey::KEY_O,
                               KeyboardKey::KEY_P,
                               KeyboardKey::KEY_Q,
                               KeyboardKey::KEY_R,
                               KeyboardKey::KEY_S,
                               KeyboardKey::KEY_T,
                               KeyboardKey::KEY_U,
                               KeyboardKey::KEY_V,
                               KeyboardKey::KEY_W,
                               KeyboardKey::KEY_X,
                               KeyboardKey::KEY_Y,
                               KeyboardKey::KEY_Z,
                               KeyboardKey::KEY_LEFT_BRACKET,
                               KeyboardKey::KEY_BACKSLASH,
                               KeyboardKey::KEY_RIGHT_BRACKET,
                               KeyboardKey::KEY_GRAVE,
                               KeyboardKey::KEY_SPACE,
                               KeyboardKey::KEY_ESCAPE,
                               KeyboardKey::KEY_ENTER,
                               KeyboardKey::KEY_TAB,
                               KeyboardKey::KEY_BACKSPACE,
                               KeyboardKey::KEY_INSERT,
                               KeyboardKey::KEY_DELETE,
                               KeyboardKey::KEY_RIGHT,
                               KeyboardKey::KEY_LEFT,
                               KeyboardKey::KEY_DOWN,
                               KeyboardKey::KEY_UP,
                               KeyboardKey::KEY_PAGE_UP,
                               KeyboardKey::KEY_PAGE_DOWN,
                               KeyboardKey::KEY_HOME,
                               KeyboardKey::KEY_END,
                               KeyboardKey::KEY_CAPS_LOCK,
                               KeyboardKey::KEY_SCROLL_LOCK,
                               KeyboardKey::KEY_NUM_LOCK,
                               KeyboardKey::KEY_PRINT_SCREEN,
                               KeyboardKey::KEY_PAUSE,
                               KeyboardKey::KEY_F1,
                               KeyboardKey::KEY_F2,
                               KeyboardKey::KEY_F3,
                               KeyboardKey::KEY_F4,
                               KeyboardKey::KEY_F5,
                               KeyboardKey::KEY_F6,
                               KeyboardKey::KEY_F7,
                               KeyboardKey::KEY_F8,
                               KeyboardKey::KEY_F9,
                               KeyboardKey::KEY_F10,
                               KeyboardKey::KEY_F11,
                               KeyboardKey::KEY_F12,
                               KeyboardKey::KEY_LEFT_SHIFT,
                               KeyboardKey::KEY_LEFT_CONTROL,
                               KeyboardKey::KEY_LEFT_ALT,
                               KeyboardKey::KEY_LEFT_SUPER,
                               KeyboardKey::KEY_RIGHT_SHIFT,
                               KeyboardKey::KEY_RIGHT_CONTROL,
                               KeyboardKey::KEY_RIGHT_ALT,
                               KeyboardKey::KEY_RIGHT_SUPER,
                               KeyboardKey::KEY_KB_MENU,
                               KeyboardKey::KEY_KP_0,
                               KeyboardKey::KEY_KP_1,
                               KeyboardKey::KEY_KP_2,
                               KeyboardKey::KEY_KP_3,
                               KeyboardKey::KEY_KP_4,
                               KeyboardKey::KEY_KP_5,
                               KeyboardKey::KEY_KP_6,
                               KeyboardKey::KEY_KP_7,
                               KeyboardKey::KEY_KP_8,
                               KeyboardKey::KEY_KP_9,
                               KeyboardKey::KEY_KP_DECIMAL,
                               KeyboardKey::KEY_KP_DIVIDE,
                               KeyboardKey::KEY_KP_MULTIPLY,
                               KeyboardKey::KEY_KP_SUBTRACT,
                               KeyboardKey::KEY_KP_ADD,
                               KeyboardKey::KEY_KP_ENTER,
                               KeyboardKey::KEY_KP_EQUAL,
                               KeyboardKey::KEY_BACK,
                               KeyboardKey::KEY_MENU,
                               KeyboardKey::KEY_VOLUME_UP,
                               KeyboardKey::KEY_VOLUME_DOWN};

  const size_t mouse_len = 7;

  MouseButton all_mouse_keys[] = {
      MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_MIDDLE,
      MOUSE_BUTTON_SIDE, MOUSE_BUTTON_EXTRA, MOUSE_BUTTON_FORWARD,
      MOUSE_BUTTON_BACK,
  };

  for (size_t i = 0; i < kb_len; i++) {
    this->keyboard_state[all_kb_keys[i]] = KeyState::Up;
  }

  for (size_t i = 0; i < mouse_len; i++) {
    this->mouse_state[all_mouse_keys[i]] = KeyState::Up;
  }

  // Will be overriden at first event poll, so any values should be fine
  this->mouse_pos.x = 0;
  this->mouse_pos.y = 0;

  MT_LOG_INFO("Window object initialized with size {}x{}", size_x, size_y);
}

Window::Window(uint32_t size_x, uint32_t size_y, std::string window_name,
               uint32_t target_fps)
    : Window::Window(size_x, size_y, window_name) {
  this->target_fps = target_fps;
}

void Window::pollEvents() {
  if (window == nullptr || !window->IsFocused())
    return;

  // Keyboard events
  for (const auto &[key, previous_key_state] : keyboard_state) {
    const KeyState current_key_state =
        raylib::Keyboard::IsKeyDown(key) ? KeyState::Down : KeyState::Up;

    if (previous_key_state != current_key_state) {
      std::shared_ptr<Event> event;

      switch (current_key_state) {
      case KeyState::Down:
        event = std::make_shared<event::KeyDownEvent>(key);
        break;
      case KeyState::Up:
        event = std::make_shared<event::KeyUpEvent>(key);
        break;
      }

      if (event != nullptr) {
        event_queue.push(event);
      }

      keyboard_state[key] = current_key_state;
    }
  }

  // Mouse events
  Vector2 current_mouse_pos = raylib::Mouse::GetPosition();

  if (current_mouse_pos.x != mouse_pos.x ||
      current_mouse_pos.y != mouse_pos.y) {
    event_queue.push(
        std::make_shared<event::MouseMoveEvent>(current_mouse_pos));

    mouse_pos.x = current_mouse_pos.x;
    mouse_pos.y = current_mouse_pos.y;
  }

  for (const auto &[button, previous_button_state] : mouse_state) {
    const KeyState current_button_state =
        raylib::Mouse::IsButtonDown(button) ? KeyState::Down : KeyState::Up;

    if (previous_button_state != current_button_state) {
      std::shared_ptr<Event> event;

      switch (current_button_state) {
      case KeyState::Down:
        event = std::make_shared<event::MouseDownEvent>(button);
        break;
      case KeyState::Up:
        event = std::make_shared<event::MouseUpEvent>(button);
        break;
      }

      if (event != nullptr) {
        event_queue.push(event);
      }

      mouse_state[button] = current_button_state;
    }
  }

  float mouse_wheel_movement = raylib::Mouse::GetWheelMove();
  if (mouse_wheel_movement != 0) {
    event_queue.push(
        std::make_shared<event::MouseWheelMoveEvent>(mouse_wheel_movement));
  }
}

size_t Window::getEventQueueSize() const { return event_queue.size(); }

std::shared_ptr<Event> Window::popEvent() {
  if (event_queue.size() == 0) {
    return nullptr;
  }

  std::shared_ptr<Event> val = event_queue.front();
  event_queue.pop();
  return val;
};

void Window::init() {
  window = std::make_unique<raylib::Window>(this->size_x, this->size_y,
                                            this->window_name);
  if (target_fps != 0) {
    window->SetTargetFPS(target_fps);
    MT_LOG_INFO("FPS set to uncapped");
  }

  MT_LOG_INFO("Window initialized", size_x, size_y);
}

void Window::die() {
  window.reset();
  MT_LOG_INFO("Window cleaned up");
}

bool Window::shouldClose() const { return window->ShouldClose(); }

float Window::getFrameTime() const { return window->GetFrameTime(); }

void Window::drawFrame(
    const std::vector<std::shared_ptr<Drawable>> &drawables) {
  window->BeginDrawing();

  window->ClearBackground(RAYWHITE);

  std::vector<std::shared_ptr<Drawable>> drawables_sorted(drawables);

  std::sort(drawables_sorted.begin(), drawables_sorted.end(),
            [](const auto &a, const auto &b) { return a->getZ() < b->getZ(); });

  for (const std::shared_ptr<Drawable> &drawable : drawables_sorted) {
    drawable->draw();
  }

  DrawFPS(0, 0);
  window->EndDrawing();
}

} // namespace engine::render
