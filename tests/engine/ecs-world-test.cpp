#include "engine/ecs/entity.hpp"
#include "engine/ecs/world.hpp"
#include <catch2/catch_all.hpp>

using namespace engine::ecs;

class ExampleComponent1 : public engine::ecs::Component {
public:
  ExampleComponent1() { component_id = "example-component-1"; }
  ~ExampleComponent1() override = default;
};

class ExampleComponent2 : public engine::ecs::Component {
public:
  ExampleComponent2() { component_id = "example-component-2"; }
  ~ExampleComponent2() override = default;
};

class ExampleComponent3 : public engine::ecs::Component {
public:
  ExampleComponent3() { component_id = "example-component-3"; }
  ~ExampleComponent3() override = default;
};

TEST_CASE("ECS world cache behaves consistently", "[ecs][cache]") {
  auto entity1 = std::make_shared<Entity>();
  entity1->addComponent(std::make_shared<ExampleComponent1>());
  entity1->addComponent(std::make_shared<ExampleComponent2>());
  entity1->addComponent(std::make_shared<ExampleComponent3>());

  auto entity2 = std::make_shared<Entity>();
  entity2->addComponent(std::make_shared<ExampleComponent1>());
  entity2->addComponent(std::make_shared<ExampleComponent2>());

  auto entity3 = std::make_shared<Entity>();
  entity3->addComponent(std::make_shared<ExampleComponent2>());
  entity3->addComponent(std::make_shared<ExampleComponent3>());

  World world;
  world.addEntity("entity-2", entity2);
  world.addEntity("entity-1", entity1);
  world.addEntity("entity-3", entity3);

  SECTION("Initial query results are correct and cached") {
    auto results =
        world.query({"example-component-2", "example-component-1"}, true);
    std::vector<std::string> ids;
    for (auto &[entity, id] : results)
      ids.push_back(id);

    REQUIRE(ids.size() == 2);
    REQUIRE_THAT(ids, Catch::Matchers::UnorderedEquals<std::string>(
                          {"entity-1", "entity-2"}));

    // Cached queries should give same result
    for (int i = 0; i < 3; ++i) {
      auto cached =
          world.query({"example-component-2", "example-component-1"}, true);
      std::vector<std::string> cached_ids;
      for (auto &[entity, id] : cached)
        cached_ids.push_back(id);
      REQUIRE_THAT(cached_ids, Catch::Matchers::UnorderedEquals(ids));
    }
  }

  SECTION("Removing component updates cache") {
    world.removeComponent("entity-1", "example-component-2");

    auto results =
        world.query({"example-component-2", "example-component-1"}, true);
    std::vector<std::string> ids;
    for (auto &[entity, id] : results)
      ids.push_back(id);

    REQUIRE(ids.size() == 1);
    REQUIRE(ids[0] == "entity-2");
  }

  SECTION("Re-adding component restores entity to query results") {
    world.removeComponent("entity-1", "example-component-2");
    world.addComponent("entity-1", std::make_shared<ExampleComponent2>());

    auto results =
        world.query({"example-component-2", "example-component-1"}, true);
    std::vector<std::string> ids;
    for (auto &[entity, id] : results)
      ids.push_back(id);

    REQUIRE(ids.size() == 2);
    REQUIRE_THAT(ids, Catch::Matchers::UnorderedEquals<std::string>(
                          {"entity-1", "entity-2"}));
  }

  SECTION("Removing entity invalidates cache") {
    world.removeEntity("entity-1");
    auto results =
        world.query({"example-component-2", "example-component-1"}, true);

    std::vector<std::string> ids;
    for (auto &[entity, id] : results)
      ids.push_back(id);

    REQUIRE(ids.size() == 1);
    REQUIRE(ids[0] == "entity-2");
  }

  SECTION("Re-adding entity reintroduces it into cache") {
    world.removeEntity("entity-1");
    world.addEntity("entity-1", entity1);

    auto results =
        world.query({"example-component-2", "example-component-1"}, true);
    std::vector<std::string> ids;
    for (auto &[entity, id] : results)
      ids.push_back(id);

    REQUIRE(ids.size() == 2);
    REQUIRE_THAT(ids, Catch::Matchers::UnorderedEquals<std::string>(
                          {"entity-1", "entity-2"}));
  }
}
