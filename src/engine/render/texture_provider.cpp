#include "texture_provider.hpp"
#include "../log/logger.hpp"

#include <cstdlib>
#include <filesystem>
#include <memory>
#include <raylib-cpp.hpp>
#include <unordered_set>

namespace engine::render {

TextureProvider::TextureProvider(std::filesystem::path root,
                                 std::filesystem::path missing_texture)
    : root(root), missing_texture(missing_texture) {
  registerPath(missing_texture_id, missing_texture);
}

TextureProvider::~TextureProvider() {
  // If you dare to destroy it...
  forceUnloadAll();
}

bool TextureProvider::registerPath(const std::string &id,
                                   const std::filesystem::path image_path) {
  if (!std::filesystem::exists(root / image_path)) {
    MT_LOG_ERROR("Failed to register path {} under id {}", image_path.string(),
                 id);
    return false;
  }

  if (registered_textures.count(id) > 0) {
    MT_LOG_INFO("Tried to register existing id, {}", id);

    if (registered_textures[id] != image_path) {
      MT_LOG_WARNING("Paths of textures are not matching, id: {}, old path: "
                     "{}, new path: {}, refusing to change anything",
                     id, registered_textures[id].string(), image_path.string());
      return false;
    }

    return true;
  }

  registered_textures[id] = root / image_path;
  MT_LOG_DEBUG("Registered path {} under id {}", image_path.string(), id);

  return true;
}

bool TextureProvider::registerPaths(
    const std::unordered_map<std::string, std::filesystem::path> &image_paths) {
  MT_LOG_DEBUG("Registering batch of size {}", image_paths.size());

  bool return_value = true;

  for (const auto &[id, image_path] : image_paths) {
    // Accumulates failures to return false if any registration missed
    return_value &= registerPath(id, image_path);
  }

  return return_value;
}

void TextureProvider::unregisterPath(const std::string &id) {
  if (registered_textures.count(id) == 0) {
    MT_LOG_WARNING("Tried to unregister path that isn't in registry by id {}",
                   id);
    return;
  }

  registered_textures.erase(id);
  MT_LOG_INFO("Unregistered by id {}", id);
}

void TextureProvider::unregisterPaths(const std::vector<std::string> &ids) {
  for (const auto &id : ids) {
    unregisterPath(id);
  }
}

bool TextureProvider::preload(const std::string &id) {
  if (registered_textures.count(id) == 0) {
    MT_LOG_ERROR("Tried to preload non-registered texture, id: {}", id);
    return false;
  }

  if (texture_cache.count(registered_textures[id]) > 0) {
    MT_LOG_DEBUG("Hit cache on id {}", id);
    return true;
  }

  try {
#ifdef _WIN32
    std::shared_ptr<Texture2D> texture =
        std::make_shared<raylib::Texture2D>(registered_textures[id].string());
#else
    std::shared_ptr<Texture2D> texture =
        std::make_shared<raylib::Texture2D>(registered_textures[id]);
#endif

    if (texture->id == 0) {
      MT_LOG_ERROR("Raylib error loading texture on id {}", id);

      return false;
    }

    texture_cache[registered_textures[id]] = texture;
    MT_LOG_INFO("Preloaded texture by id {}", id);

    return true;

  } catch (const raylib::RaylibException &e) {
    MT_LOG_ERROR("Raylib error loading texture on id {}, error: {}", id,
                 e.what());

    return false;
  }
}

bool TextureProvider::preload(const std::vector<std::string> &ids) {
  MT_LOG_DEBUG("Preloading batch of size {}", ids.size());

  bool return_value = true;

  for (const auto &id : ids) {
    // Accumulates failures to return false if any preload missed
    return_value &= preload(id);
  }

  return return_value;
}

std::shared_ptr<Texture2D> TextureProvider::load(const std::string &id) {
  if (registered_textures.count(id) == 0) {
    if (id == missing_texture_id) {
      // Since registration already checks existence, we can only rant here

      MT_LOG_CRITICAL(
          "Alright, how the fuck did you lose a texture FOR A MISSING TEXTURE? "
          "Are you as dumb as I think or is your filesystem corrupted? Either "
          "way I'm done, I'm crashing this entire thing until you actually "
          "make {} a valid image THAT I CAN LOAD WHEN YOUR DUMB HEAD FORGETS "
          "THAT YOU DIDN'T ADD A TEXTURE FOR SOMETHING YET AGAIN",
          (root / missing_texture).string());
      exit(1);
    }

    MT_LOG_ERROR("Trying to load unregistered texture with id {}, loading "
                 "missing texture",
                 id);
    return load(missing_texture_id);
  }

  if (!preload(id)) {
    if (id == missing_texture_id) {
      MT_LOG_CRITICAL(
          "You really want this to blow up, don't you? If not, WHY THE FUCK IS "
          "{} NOT A VALID IMAGE? WHAT DID I TELL YOU LAST TIME?",
          (root / missing_texture).string());
      exit(1);
    }

    MT_LOG_ERROR("Texture with id {} failed to load, loading missing texture",
                 id);
    return load(missing_texture_id);
  }

  return texture_cache[registered_textures[id]];
}

void TextureProvider::unload(const std::string &id) {
  if (registered_textures.count(id) == 0) {
    MT_LOG_WARNING("Trying to unload unregistered texture");
    return;
  }

  if (texture_cache.count(registered_textures[id]) == 0) {
    MT_LOG_WARNING("Trying to unload texture that is not loaded or already "
                   "marked for unloading");
    return;
  }

  std::shared_ptr<Texture2D> texture = texture_cache[registered_textures[id]];
  to_unload[id] = texture;
  texture_cache.erase(registered_textures[id]);
}

void TextureProvider::unload(const std::vector<std::string> &ids) {
  for (const auto &id : ids) {
    unload(id);
  }
}

void TextureProvider::unloadAll() {
  std::vector<std::string> to_erase;

  for (const auto &[id, full_path] : registered_textures) {
    if (texture_cache.count(full_path) > 0) {
      MT_LOG_DEBUG("Found cached texture under id {}, marking for unloading",
                   id);

      to_unload[id] = texture_cache[full_path];
#ifdef _WIN32
      to_erase.push_back(full_path.string());
#else
      to_erase.push_back(full_path);
#endif
    }
  }

  for (const auto &full_path : to_erase) {
    texture_cache.erase(full_path);
  }
}

void TextureProvider::collectUnloaded() {
  std::vector<std::string> to_erase;

  for (const auto &[id, texture] : to_unload) {
    long count = texture.use_count();
    MT_LOG_DEBUG("Checking texture with {} references and id {}", count,
                 texture->id);

    if (count == 1) {
      MT_LOG_DEBUG("Unloading texture {} and marking id {} for deletion",
                   texture->id, id);
      UnloadTexture(*texture);
      to_erase.push_back(id);
    }
  }

  for (const auto &id : to_erase) {
    MT_LOG_DEBUG("Unloading texture by id {}", id);
    to_unload.erase(id);
  }
}

void TextureProvider::forceUnload(const std::string &id) {
  if (registered_textures.count(id) == 0) {
    MT_LOG_WARNING(
        "Trying to forcefully unload texture that isn't present in registry");
    return;
  }

  // Unnecessary, but I prefer to be sure that it won't fuck up
  bool unloaded = false;

  if (texture_cache.count(registered_textures[id]) > 0) {
    MT_LOG_INFO("Unloading from cache by id {}", id);
    UnloadTexture(*texture_cache[registered_textures[id]]);
    texture_cache.erase(registered_textures[id]);
    unloaded = true;
  }

  if (to_unload.count(id) > 0) {
    if (!unloaded) {
      // TODO: fix wording when my brain works
      MT_LOG_INFO("Unloading from marked for unloading by id {}", id);
      UnloadTexture(*to_unload[id]);
    }
    to_unload.erase(id);
  }

  if (!unloaded) {
    MT_LOG_INFO("Nothing was unloaded by id {}", id);
  }
}

void TextureProvider::forceUnload(const std::vector<std::string> &ids) {
  for (const auto &id : ids) {
    forceUnload(id);
  }
}

void TextureProvider::forceUnloadAll() {
  // Be not afraid, these are ids from Texture2D and they have nothing to do
  // with strings
  std::unordered_set<unsigned int> unloaded_ids;

  for (const auto &[full_path, texture] : texture_cache) {
    // I have no idea how you could possibly not pass this statement, but if you
    // could, we are prepared
    if (unloaded_ids.find(texture->id) == unloaded_ids.end()) {
      MT_LOG_INFO("Forcefully unloading texture with id {}", texture->id);
      unloaded_ids.insert(texture->id);
      UnloadTexture(*texture);
    }
  }

  for (const auto &[id, texture] : to_unload) {
    if (unloaded_ids.find(texture->id) == unloaded_ids.end()) {
      MT_LOG_INFO("Forcefully unloading texture with id {}", texture->id);
      unloaded_ids.insert(texture->id);
      UnloadTexture(*texture);
    }
  }

  MT_LOG_DEBUG("Clearing cache and to_unload");
  texture_cache.clear();
  to_unload.clear();
}

} // namespace engine::render
