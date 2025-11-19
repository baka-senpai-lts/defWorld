#include "input_system.hpp"

#include "engine/events/event.hpp"

#include "engine/events/key_down.hpp"
#include "engine/events/key_up.hpp"
#include "engine/events/mouse_down.hpp"
#include "engine/events/mouse_move.hpp"
#include "engine/events/mouse_up.hpp"
#include "engine/events/mouse_wheel_move.hpp"

#include "../components/emit_particles_component.hpp"
#include "../components/position_component.hpp"
#include "../components/sprite_component.hpp"
#include "game/components/follow_cursor_component.hpp"
#include "game/systems/emit_particles_system.hpp"

void InputSystem::run(engine::ecs::World &world, float delta) {
  window->pollEvents();

  auto entities =
      world.query<FollowCursorComponent, PositionComponent, SpriteComponent>();

  std::shared_ptr<engine::Event> event = window->popEvent();
  while (event != nullptr) {
    switch (event->getEventType()) {
    case engine::EventType::KEY_UP:
      break;

    case engine::EventType::KEY_DOWN:
      break;

    case engine::EventType::MOUSE_UP:
      for (const auto &[_, id] : entities) {
        world.removeComponent<EmitParticlesComponent>(id);
      }
      break;

    case engine::EventType::MOUSE_DOWN:
      for (const auto &[_, id] : entities) {
        world.addComponent<EmitParticlesComponent>(id);
      }
      break;

    case engine::EventType::MOUSE_MOVE:
      for (const auto &[entity, _] : entities) {
        auto position = entity->get<PositionComponent>();

        position->pos.x =
            std::static_pointer_cast<engine::event::MouseMoveEvent>(event)
                ->getX();

        position->pos.y =
            std::static_pointer_cast<engine::event::MouseMoveEvent>(event)
                ->getY();
      }
      break;

    case engine::EventType::MOUSE_WHEEL_MOVE:
      for (const auto &[entity, _] : entities) {
        auto sprite = entity->get<SpriteComponent>()->sprite;

        sprite->setScale(
            sprite->getScale() +
            (std::static_pointer_cast<engine::event::MouseWheelMoveEvent>(event)
                 ->getWheelMovement() *
             0.1));
      }
      break;

    default:
      break;
    }
    event = window->popEvent();
  }
}
