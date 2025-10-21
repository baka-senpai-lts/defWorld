#include "raylib-cpp.hpp" // IWYU pragma: keep

int main() {
  raylib::Window window(800, 600, "My Game");

  SetTargetFPS(60);

  while (!window.ShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Hello Raylib-cpp!", 200, 280, 20, BLACK);
    EndDrawing();
  }
  return 0;
}
