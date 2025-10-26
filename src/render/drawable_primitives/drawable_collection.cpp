#include "drawable_collection.hpp"

namespace game::render {

DrawableCollection::DrawableCollection(
    std::unordered_map<std::string, std::shared_ptr<Drawable>> drawables,
    Vector2 pos)
    : Drawable(pos), drawables(drawables) {}

void DrawableCollection::set(std::string key,
                             std::shared_ptr<Drawable> drawable) {
  drawables[key] = drawable;
}

void DrawableCollection::remove(const std::string &key) {
  // No need for additional checks, it's already safe because of
  // std::unordered_map::erase implementation
  drawables.erase(key);
};

std::shared_ptr<Drawable> DrawableCollection::get(const std::string &key) {
  if (drawables.count(key) > 0) {
    return drawables[key];
  }

  return nullptr;
}

void DrawableCollection::draw() const {
  for (const auto &[_, drawable] : drawables) {
    drawable->draw(pos);
  }
}

} // namespace game::render
