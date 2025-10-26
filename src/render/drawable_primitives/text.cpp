#include "text.hpp"

#include <raylib.h>

namespace game::render {

Text::Text(std::string str, Vector2 pos, int font_size)
    : Drawable(pos), str(str), font_size(font_size), color(BLACK) {}

Text::Text(std::string str, Vector2 pos, int font_size, Color color)
    : Text(str, pos, font_size) {
  this->color = color;
}

void Text::draw() const {
  DrawText(str.c_str(), pos.x, pos.y, font_size, color);
}

} // namespace game::render
