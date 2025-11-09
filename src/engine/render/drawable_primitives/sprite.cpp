#include "sprite.hpp"
#include <cmath>
#include <raylib.h>

namespace engine::render {

void Sprite::setScale(float scale) {
  this->scale = scale > 0 ? scale : 0;
  origin = {(float)texture->width * scale / 2,
            (float)texture->height * scale / 2};
}

void Sprite::draw() const {
  Rectangle sourceRec = {0, 0, (float)texture->width, (float)texture->height};
  Rectangle destRec = {pos.x, pos.y, texture->width * scale,
                       texture->height * scale};

  DrawTexturePro(*texture, sourceRec, destRec, origin, rotation, tint);
}

} // namespace engine::render
