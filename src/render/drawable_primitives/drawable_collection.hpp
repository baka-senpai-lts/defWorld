#ifndef DRAWABLE_COLLECTION_H_
#define DRAWABLE_COLLECTION_H_

#include "../drawable.hpp"

#include <memory>
#include <raylib.h>
#include <string>
#include <unordered_map>

namespace game::render {

class DrawableCollection : public Drawable {
private:
  std::unordered_map<std::string, std::shared_ptr<Drawable>> drawables;

public:
  DrawableCollection(
      std::unordered_map<std::string, std::shared_ptr<Drawable>> drawables,
      Vector2 pos);
  ~DrawableCollection() override {};

  void set(std::string key, std::shared_ptr<Drawable> drawable);
  void remove(const std::string &key);
  std::shared_ptr<Drawable> get(const std::string &key);

  void draw() const override;
};

} // namespace game::render

#endif // DRAWABLE_COLLECTION_H_
