#include "text.hpp"

#include <raylib.h>

namespace engine::render {

Text::Text(std::string str, Vector2 pos, int font_size, int z)
    : Drawable(pos, z), str(str), font_size(font_size), color(BLACK) {}

Text::Text(std::string str, Vector2 pos, int font_size, Color color, int z)
    : Text(str, pos, font_size, z) {
  this->color = color;
}

void Text::draw() const {
  DrawText(str.c_str(), pos.x, pos.y, font_size, color);
}

} // namespace engine::render
