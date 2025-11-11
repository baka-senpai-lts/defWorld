#include "game.hpp"

#include "../ecs/system.hpp"
#include "../ecs/world.hpp"
#include "../log/logger.hpp"

#include <memory>

namespace engine::base {

Game::Game(uint32_t size_x, uint32_t size_y, std::string window_name,
           uint32_t target_fps) {
  window =
      std::make_shared<render::Window>(size_x, size_y, window_name, target_fps);
}

void Game::addSystem(std::shared_ptr<ecs::System> system) {
  const std::string &id = system->getID();

  if (systems.count(id) > 0) {
    MT_LOG_WARNING("Passed system with existing id {}, proceeding to overwrite",
                   id);
  }

  this->systems[id] = std::move(system);
}

void Game::removeSystem(const std::string &id) { this->systems.erase(id); }

void Game::init() {
  window->init();
  this->world = std::make_shared<ecs::World>();
}

void Game::run() {
  while (!window->shouldClose()) {
    scheduler.run(systems, *world, window->getFrameTime());
  }
}

void Game::die() {
  world->clear();
  window->die();
}

} // namespace engine::base
