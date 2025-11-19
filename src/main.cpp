#include "engine/base/game.hpp"
#include "engine/log/logger.hpp"
#include "engine/render/texture_provider.hpp"

#include "engine/util/entity_builder.hpp"

#include "game/components/follow_cursor_component.hpp"
#include "game/components/position_component.hpp"
#include "game/components/sprite_component.hpp"

#include "game/systems/drawable_position_system.hpp"
#include "game/systems/emit_particles_system.hpp"
#include "game/systems/input_system.hpp"
#include "game/systems/particle_system.hpp"
#include "game/systems/render_system.hpp"

#include <memory>

void setupWorld(
    std::shared_ptr<engine::ecs::World> world,
    std::shared_ptr<engine::render::TextureProvider> texture_provider) {
  world->addEntity(
      "simple-sprite-entity",
      engine::util::EntityBuilder()
          .with<SpriteComponent>(texture_provider->load("sample-sprite"),
                                 Vector2{100, 100}, 5)
          .with<PositionComponent>()
          .with<FollowCursorComponent>()
          .assemble());
}

void setupSystems(
    engine::base::Game &game,
    std::shared_ptr<engine::render::TextureProvider> texture_provider) {
  game.addWindowSystem<RenderSystem>();
  game.addWindowSystem<InputSystem>();
  game.addSystem<DrawablePositionSystem>();
  game.addSystem<EmitParticlesSystem>(texture_provider);
  game.addSystem<ParticleSystem>();
}

int main() {
  SetTraceLogLevel(LOG_NONE); // Disabling raylib's logger
  engine::log::Logger::getInstance().setLogLevel(engine::log::LogLevel::DEBUG);
  engine::log::Logger::getInstance().setWriteFile(false);

  engine::base::Game game(800, 600, "My Game");

  {
    std::shared_ptr<engine::render::TextureProvider> texture_provider =
        std::make_shared<engine::render::TextureProvider>("./assets/");
    texture_provider->registerPath("sample-sprite", "./sample-sprite.png");

    game.init();

    setupWorld(game.getWorld(), texture_provider);
    setupSystems(game, texture_provider);

    game.run();

    texture_provider->forceUnloadAll();
  }

  return 0;
}
