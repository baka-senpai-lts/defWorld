#ifndef TEXT_H_
#define TEXT_H_

#include "../drawable.hpp"

#include <raylib.h>
#include <string>

namespace engine::render {

class Text : public Drawable {
private:
  std::string str;
  int font_size;
  Color color;

public:
  Text(std::string str, Vector2 pos, int font_size, int z = 0);
  Text(std::string str, Vector2 pos, int font_size, Color color, int z = 0);

  ~Text() override {};

  void draw() const override;
};

} // namespace engine::render

#endif // TEXT_H_
