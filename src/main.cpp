#include "raylib-cpp.hpp" // IWYU pragma: keep
#include "render/window.hpp"

int main() {
  game::render::Window window(800, 600, "My Game");

  window.init();
  while (!window.shouldClose()) {
    window.drawFrame();
  }
  window.die();

  return 0;
}
