#ifndef TEXTURE_PROVIDER_H_
#define TEXTURE_PROVIDER_H_

#include <filesystem>
#include <memory>
#include <raylib.h>
#include <unordered_map>
#include <vector>

namespace engine::render {

// NOTE: Only use AFTER initializing a window
class TextureProvider {
private:
  // Contains paths to sprites by registered IDs
  std::unordered_map<std::string, std::filesystem::path> registered_textures;
  std::unordered_map<std::filesystem::path, std::shared_ptr<Texture2D>>
      texture_cache;

  std::unordered_map<std::string, std::shared_ptr<Texture2D>> to_unload;

  std::filesystem::path missing_texture;
  std::filesystem::path root;

  const std::string missing_texture_id = "missing-texture";

public:
  TextureProvider(std::filesystem::path root,
                  std::filesystem::path missing_texture = "./404.png");

  // Destroying me would cause forceful unloading of *EVERYTHING*
  ~TextureProvider();

  // Returns false if it didn't found requested path
  bool registerPath(const std::string &id,
                    const std::filesystem::path image_path);

  // Takes map of id-path, returns false if search for any file failed
  bool
  registerPaths(const std::unordered_map<std::string, std::filesystem::path>
                    &image_paths);

  // Unregisters path and marks texture for unloading
  void unregisterPath(const std::string &id);

  // Unregisters paths and marks textures for unloading
  void unregisterPaths(const std::vector<std::string> &ids);

  // Preloads texture, returns false if failed
  bool preload(const std::string &id);
  bool preload(const std::vector<std::string> &id);

  // Loads texture and returns it, returns nullptr if failed
  std::shared_ptr<Texture2D> load(const std::string &id);

  // Unloads texture by id, will not unload until only TextureProvider contains
  // instances to each one of them, but texture will not be available in
  // registry
  void unload(const std::string &id);
  void unload(const std::vector<std::string> &ids);

  // Unloads all textures, will not unload until only TextureProvider contains
  // instances to each one of them, but texture will not be available in
  // registry
  void unloadAll();

  // Forcefully unloads texture (UNSAFE)
  void forceUnload(const std::string &id);
  void forceUnload(const std::vector<std::string> &ids);

  // Forcefully unloads all textures (UNSAFE)
  void forceUnloadAll();

  // Unloads textures marked for unloading
  void collectUnloaded();
};

} // namespace engine::render

#endif // TEXTURE_PROVIDER_H_
