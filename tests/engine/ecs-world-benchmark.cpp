#include "engine/ecs/entity.hpp"
#include "engine/ecs/world.hpp"
#include "engine/log/logger.hpp"

#include <catch2/catch_all.hpp>
#include <random>

using namespace engine::ecs;

class ExampleComponent1 : public Component {
public:
  ExampleComponent1() { component_id = "example-component-1"; }
  ~ExampleComponent1() override = default;
};

class ExampleComponent2 : public Component {
public:
  ExampleComponent2() { component_id = "example-component-2"; }
  ~ExampleComponent2() override = default;
};

class ExampleComponent3 : public Component {
public:
  ExampleComponent3() { component_id = "example-component-3"; }
  ~ExampleComponent3() override = default;
};

TEST_CASE("ECS world benchmarks", "[ecs][benchmark]") {
  constexpr size_t entity_count = 1000;

  BENCHMARK("Query entities with 2 components") {
    World world;
    for (size_t i = 0; i < entity_count; ++i) {
      auto entity = std::make_shared<Entity>();
      if (i % 3 == 0)
        entity->addComponent(std::make_shared<ExampleComponent1>());
      if (i % 2 == 0)
        entity->addComponent(std::make_shared<ExampleComponent2>());
      entity->addComponent(std::make_shared<ExampleComponent3>());
      world.addEntity("entity-" + std::to_string(i), entity);
    }
    auto results =
        world.query({"example-component-1", "example-component-2"}, true);
    return results.size();
  };

  BENCHMARK("Query entities with 1 component") {
    World world;
    for (size_t i = 0; i < entity_count; ++i) {
      auto entity = std::make_shared<Entity>();
      if (i % 3 == 0)
        entity->addComponent(std::make_shared<ExampleComponent1>());
      if (i % 2 == 0)
        entity->addComponent(std::make_shared<ExampleComponent2>());
      entity->addComponent(std::make_shared<ExampleComponent3>());
      world.addEntity("entity-" + std::to_string(i), entity);
    }
    auto results = world.query({"example-component-3"}, true);
    return results.size();
  };

  BENCHMARK("Add a new entity") {
    World world;
    for (size_t i = 0; i < entity_count; ++i) {
      auto entity = std::make_shared<Entity>();
      entity->addComponent(std::make_shared<ExampleComponent1>());
      entity->addComponent(std::make_shared<ExampleComponent2>());
      world.addEntity("entity-" + std::to_string(i), entity);
    }
    auto new_entity = std::make_shared<Entity>();
    new_entity->addComponent(std::make_shared<ExampleComponent1>());
    new_entity->addComponent(std::make_shared<ExampleComponent2>());
    world.addEntity("new-entity", new_entity);
    return world.query({"example-component-1"}, true).size();
  };

  BENCHMARK("Remove a component from an entity") {
    World world;
    for (size_t i = 0; i < entity_count; ++i) {
      auto entity = std::make_shared<Entity>();
      entity->addComponent(std::make_shared<ExampleComponent1>());
      entity->addComponent(std::make_shared<ExampleComponent2>());
      world.addEntity("entity-" + std::to_string(i), entity);
    }
    world.removeComponent("entity-0", "example-component-2");
    return world.query({"example-component-2"}, true).size();
  };

  BENCHMARK("Remove and re-add entity") {
    World world;
    for (size_t i = 0; i < entity_count; ++i) {
      auto entity = std::make_shared<Entity>();
      entity->addComponent(std::make_shared<ExampleComponent1>());
      world.addEntity("entity-" + std::to_string(i), entity);
    }
    auto removed_entity =
        world.query({"example-component-1"}, true).front().first;
    world.removeEntity("entity-0");
    world.addEntity("entity-0", std::make_shared<Entity>());
    return world.query({"example-component-1"}, true).size();
  };
}

TEST_CASE("ECS world cache benchmarks", "[ecs][benchmark]") {
  constexpr size_t entity_count = 1000;

  BENCHMARK_ADVANCED("Cached query performance")(
      Catch::Benchmark::Chronometer meter) {
    // Setup: build the world once per iteration
    World world;
    for (size_t i = 0; i < entity_count; ++i) {
      auto entity = std::make_shared<Entity>();
      if (i % 3 == 0)
        entity->addComponent(std::make_shared<ExampleComponent1>());
      if (i % 2 == 0)
        entity->addComponent(std::make_shared<ExampleComponent2>());
      entity->addComponent(std::make_shared<ExampleComponent3>());
      world.addEntity("entity-" + std::to_string(i), entity);
    }

    // Warm up the cache
    world.query({"example-component-1", "example-component-2"}, true);

    meter.measure([&world]() {
      // Timed: cached query should be very fast
      auto results =
          world.query({"example-component-1", "example-component-2"}, true);
      return results.size();
    });
  };

  BENCHMARK_ADVANCED("Cache invalidation after component removal")(
      Catch::Benchmark::Chronometer meter) {
    World world;
    for (size_t i = 0; i < entity_count; ++i) {
      auto entity = std::make_shared<Entity>();
      if (i % 3 == 0)
        entity->addComponent(std::make_shared<ExampleComponent1>());
      if (i % 2 == 0)
        entity->addComponent(std::make_shared<ExampleComponent2>());
      entity->addComponent(std::make_shared<ExampleComponent3>());
      world.addEntity("entity-" + std::to_string(i), entity);
    }

    // Warm up cache
    world.query({"example-component-1", "example-component-2"}, true);

    meter.measure([&world]() {
      world.removeComponent("entity-0", "example-component-2");

      auto results =
          world.query({"example-component-1", "example-component-2"}, true);

      world.addComponent("entity-0", std::make_shared<ExampleComponent2>());

      return results.size();
    });
  };
}

TEST_CASE("ECS incremental cache benchmarks", "[ecs][benchmark]") {
  constexpr size_t entity_count = 1000;

  BENCHMARK_ADVANCED("Cached query performance (incremental cache)")(
      Catch::Benchmark::Chronometer meter) {
    World world;

    // Setup: add entities and components
    for (size_t i = 0; i < entity_count; ++i) {
      auto entity = std::make_shared<Entity>();
      if (i % 3 == 0)
        entity->addComponent(std::make_shared<ExampleComponent1>());
      if (i % 2 == 0)
        entity->addComponent(std::make_shared<ExampleComponent2>());
      entity->addComponent(std::make_shared<ExampleComponent3>());
      world.addEntity("entity-" + std::to_string(i), entity);
    }

    // Warm the cache for the target query
    world.query({"example-component-1", "example-component-2"}, true);

    meter.measure([&world]() {
      // Read cached query results
      auto results =
          world.query({"example-component-1", "example-component-2"}, true);
      return results.size();
    });
  };

  BENCHMARK_ADVANCED("Incremental cache update: remove/add component")(
      Catch::Benchmark::Chronometer meter) {
    World world;

    // Setup: add entities and components
    for (size_t i = 0; i < entity_count; ++i) {
      auto entity = std::make_shared<Entity>();
      if (i % 3 == 0)
        entity->addComponent(std::make_shared<ExampleComponent1>());
      if (i % 2 == 0)
        entity->addComponent(std::make_shared<ExampleComponent2>());
      entity->addComponent(std::make_shared<ExampleComponent3>());
      world.addEntity("entity-" + std::to_string(i), entity);
    }

    // Warm the cache
    world.query({"example-component-1", "example-component-2"}, true);

    meter.measure([&world]() {
      // Incremental removal
      world.removeComponent("entity-0", "example-component-2");

      // Incremental addition
      world.addComponent("entity-0", std::make_shared<ExampleComponent2>());

      // Optional: return the query size to ensure some work
      return world.query({"example-component-1", "example-component-2"}, true)
          .size();
    });
  };

  BENCHMARK_ADVANCED("Incremental cache update: remove/add entity")(
      Catch::Benchmark::Chronometer meter) {
    World world;

    // Setup: add entities and components
    for (size_t i = 0; i < entity_count; ++i) {
      auto entity = std::make_shared<Entity>();
      if (i % 3 == 0)
        entity->addComponent(std::make_shared<ExampleComponent1>());
      if (i % 2 == 0)
        entity->addComponent(std::make_shared<ExampleComponent2>());
      entity->addComponent(std::make_shared<ExampleComponent3>());
      world.addEntity("entity-" + std::to_string(i), entity);
    }

    // Warm the cache
    world.query({"example-component-1", "example-component-2"}, true);

    meter.measure([&world]() {
      // Incremental entity removal
      auto removed_entity =
          world.query({"example-component-1"}, true).front().first;
      world.removeEntity("entity-0");

      // Incremental entity addition
      auto entity = std::make_shared<Entity>();
      entity->addComponent(std::make_shared<ExampleComponent1>());
      entity->addComponent(std::make_shared<ExampleComponent2>());
      world.addEntity("entity-0", entity);

      // Optional: query to ensure some work
      return world.query({"example-component-1", "example-component-2"}, true)
          .size();
    });
  };
}

TEST_CASE("ECS incremental cache stress test", "[ecs][benchmark]") {
  constexpr size_t entity_count = 10000;
  constexpr size_t mutations_per_iteration = 500;
  constexpr size_t queries_per_iteration = 50;

  engine::log::Logger::getInstance().setLogLevel(engine::log::LogLevel::NONE);

  std::mt19937 rng(42); // fixed seed for reproducibility
  std::uniform_int_distribution<size_t> entity_dist(0, entity_count - 1);
  std::uniform_int_distribution<int> component_dist(1, 3);

  BENCHMARK_ADVANCED("Stress test: random mutations and cached queries")(
      Catch::Benchmark::Chronometer meter) {
    World world;

    // Setup: add 10k entities with varying components
    for (size_t i = 0; i < entity_count; ++i) {
      auto entity = std::make_shared<Entity>();
      if (i % 3 == 0)
        entity->addComponent(std::make_shared<ExampleComponent1>());
      if (i % 2 == 0)
        entity->addComponent(std::make_shared<ExampleComponent2>());
      entity->addComponent(std::make_shared<ExampleComponent3>());
      world.addEntity("entity-" + std::to_string(i), entity);
    }

    // Warm up the cache
    world.query({"example-component-1", "example-component-2"}, true);

    meter.measure([&world, &rng, entity_count, mutations_per_iteration,
                   queries_per_iteration]() {
      // Perform random component mutations
      for (size_t i = 0; i < mutations_per_iteration; ++i) {
        size_t entity_index = rng() % entity_count;
        std::string entity_id = "entity-" + std::to_string(entity_index);
        int comp = rng() % 3;
        switch (comp) {
        case 0:
          world.removeComponent(entity_id, "example-component-1");
          world.addComponent(entity_id, std::make_shared<ExampleComponent1>());
          break;
        case 1:
          world.removeComponent(entity_id, "example-component-2");
          world.addComponent(entity_id, std::make_shared<ExampleComponent2>());
          break;
        case 2:
          world.removeComponent(entity_id, "example-component-3");
          world.addComponent(entity_id, std::make_shared<ExampleComponent3>());
          break;
        }
      }

      // Perform multiple cached queries
      size_t total_results = 0;
      for (size_t q = 0; q < queries_per_iteration; ++q) {
        auto results =
            world.query({"example-component-1", "example-component-2"}, true);
        total_results += results.size();
      }

      return total_results;
    });
  };

  engine::log::Logger::getInstance().setLogLevel(engine::log::LogLevel::INFO);
}
