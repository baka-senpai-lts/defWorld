#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include <raylib.h>

namespace game::render {

class Drawable {
protected:
  Vector2 pos;
  int z;

public:
  Drawable(int z = 0) : z(0) {
    pos.x = 0;
    pos.y = 0;
  }

  Drawable(float x, float y, int z = 0) : z(z) {
    pos.x = x;
    pos.y = y;
  }

  Drawable(Vector2 pos, int z = 0) : pos(pos), z(0) {}

  virtual ~Drawable() {}

  float getX() const { return pos.x; }
  float getY() const { return pos.y; }
  float getZ() const { return z; }

  Vector2 getPos() const { return pos; };

  void setX(float x) { pos.x = x; }
  void setY(float y) { pos.y = y; }
  void setZ(float z) { z = z; }

  void setPos(Vector2 new_pos) { pos = new_pos; }

  virtual void draw() const = 0;

  void draw(const Vector2 &offset) {
    // I know it's shit, but it works
    pos.x += offset.x;
    pos.y += offset.y;
    draw();
    pos.x -= offset.x;
    pos.y -= offset.y;
  };
};

} // namespace game::render

#endif // DRAWABLE_H_
