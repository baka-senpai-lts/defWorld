#include "sprite.hpp"
#include <cmath>
#include <raylib.h>

namespace engine::render {

void Sprite::draw() const {
  Rectangle sourceRec = {0, 0, (float)texture->width, (float)texture->height};
  Rectangle destRec = {pos.x, pos.y, texture->width * scale,
                       texture->height * scale};

  DrawTexturePro(*texture, sourceRec, destRec, origin, rotation, tint);
}

} // namespace engine::render
