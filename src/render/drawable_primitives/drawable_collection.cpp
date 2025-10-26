#include "drawable_collection.hpp"
#include <algorithm>
#include <memory>
#include <vector>

namespace game::render {

DrawableCollection::DrawableCollection(
    std::unordered_map<std::string, std::shared_ptr<Drawable>> drawables,
    Vector2 pos, int z)
    : Drawable(pos, z), drawables(drawables) {}

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
  std::vector<std::shared_ptr<Drawable>> drawables_vec;
  drawables_vec.reserve(drawables.size());

  for (const auto &[_, drawable] : drawables) {
    drawables_vec.push_back(drawable);
  }

  std::sort(drawables_vec.begin(), drawables_vec.end(),
            [](const auto &a, const auto &b) { return a->getZ() < b->getZ(); });

  for (const std::shared_ptr<Drawable> &drawable : drawables_vec) {
    drawable->draw();
  }
}

} // namespace game::render
