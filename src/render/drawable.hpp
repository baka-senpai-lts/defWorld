#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include <raylib.h>

namespace game::render {

class Drawable {
protected:
  Vector2 pos;

public:
  Drawable() {
    pos.x = 0;
    pos.y = 0;
  }

  Drawable(float x, float y) {
    pos.x = x;
    pos.y = y;
  }

  Drawable(Vector2 pos) : pos(pos) {}

  virtual ~Drawable() {}

  float getX() const { return pos.x; }
  float getY() const { return pos.y; }

  Vector2 getPos() const { return pos; };

  void setX(float x) { pos.x = x; }
  void setY(float y) { pos.y = y; }

  void setPos(Vector2 new_pos) { pos = new_pos; }

  virtual void draw() = 0;
};

} // namespace game::render

#endif // DRAWABLE_H_
