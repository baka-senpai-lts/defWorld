#ifndef TEXT_H_
#define TEXT_H_

#include "../drawable.hpp"

#include <raylib.h>
#include <string>

namespace game::render {

class Text : public Drawable {
private:
  std::string str;
  int font_size;
  Color color;

public:
  Text(std::string str, Vector2 pos, int font_size);
  Text(std::string str, Vector2 pos, int font_size, Color color);

  ~Text() override {};

  void draw() override;
};

} // namespace game::render

#endif // TEXT_H_
