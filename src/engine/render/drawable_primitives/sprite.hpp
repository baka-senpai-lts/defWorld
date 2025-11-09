#ifndef SPRITE_H_
#define SPRITE_H_

#include "../../log/logger.hpp"
#include "../drawable.hpp"

#include <cmath>
#include <memory>
#include <raylib.h>

namespace engine::render {

class Sprite : public Drawable {
private:
  std::shared_ptr<Texture> texture;
  float rotation;
  float scale;
  Color tint;
  Vector2 origin;

public:
  Sprite(std::shared_ptr<Texture> texture, Vector2 pos, float rotation = 0,
         Color tint = WHITE, float scale = 1, int z = 0)
      : Drawable(pos, z), rotation(fmod(rotation, 360)), scale(scale),
        texture(texture), tint(tint) {
    MT_LOG_DEBUG("Initialized sprite with texture size {}x{}", texture->height,
                 texture->width);

    // Default to center
    origin = {(float)texture->width * scale / 2,
              (float)texture->height * scale / 2};
  }

  ~Sprite() override {};

  void setTexture(std::shared_ptr<Texture> texture) { this->texture = texture; }
  void setRotation(float rotation) { this->rotation = fmod(rotation, 360); }
  void setScale(float scale) {
    this->scale = scale;
    origin = {(float)texture->width * scale / 2,
              (float)texture->height * scale / 2};
  }
  void setTint(Color tint) { this->tint = tint; }
  void setOrigin(Vector2 origin) { this->origin = origin; }

  inline std::shared_ptr<Texture> getTexture() const { return texture; }
  inline float getRotation() const { return rotation; }
  inline float getScale() const { return scale; }
  inline Color getTint() const { return tint; }
  inline Vector2 getOrigin() const { return origin; }

  void draw() const override;
};

} // namespace engine::render

#endif // SPRITE_H_
