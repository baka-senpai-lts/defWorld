#ifndef GAME_H_
#define GAME_H_

#include "../ecs/system.hpp"
#include "../ecs/world.hpp"
#include "../render/window.hpp"

#include "system_scheduler.hpp"

#include <memory>

namespace engine::base {

class Game {
private:
  std::shared_ptr<render::Window> window;

  std::unordered_map<std::string, std::shared_ptr<ecs::System>> systems;
  std::shared_ptr<ecs::World> world;

  SystemScheduler scheduler;

public:
  Game(uint32_t size_x, uint32_t size_y, std::string window_name,
       uint32_t target_fps = 0);
  ~Game() = default;

  void addSystem(std::shared_ptr<ecs::System> system);

  template <class T, typename... Args>
  inline void addWindowSystem(Args &&...args) {
    static_assert(std::is_base_of_v<ecs::System, T>,
                  "T must derive from engine::ecs::System");
    addSystem(std::make_shared<T>(window, std::forward<Args>(args)...));
  }

  template <class T, typename... Args> inline void addSystem(Args &&...args) {
    static_assert(std::is_base_of_v<ecs::System, T>,
                  "T must derive from engine::ecs::System");
    addSystem(std::make_shared<T>(std::forward<Args>(args)...));
  }

  inline std::shared_ptr<ecs::World> getWorld() const { return world; }

  void removeSystem(const std::string &id);

  void init();

  // Main game loop
  void run();

  void die();
};

} // namespace engine::base

#endif // GAME_H_
