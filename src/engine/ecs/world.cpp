#include "world.hpp"
#include "../log/logger.hpp"
#include <memory>
#include <unordered_set>
#include <vector>

namespace engine::ecs {

World::World() : next_component_bit(0) {}

size_t World::componentIDToBit(const std::string &id) {
  auto existing_bit = component_id_to_bit.find(id);

  if (existing_bit != component_id_to_bit.end()) {
    return existing_bit->second;
  }

  if (next_component_bit >= bitset_size) {
    MT_LOG_CRITICAL("Component bitmask overflow ({} >= {}), fix bitset_size in "
                    "World class definition. AGAIN GO AND FIX IT RIGHT NOW, "
                    "IT'S NOT GOING TO WORK OTHERWISE, SO I'M CRASHING",
                    next_component_bit, bitset_size);
    // TODO: maybe figure out nicer way to do this... Or don't, I shouldn't be
    // nice with someone who does THIS
    exit(1);
  }

  size_t current_bit = next_component_bit;
  MT_LOG_DEBUG("Assigning {} component id to bit {}", id, current_bit);
  component_id_to_bit[id] = current_bit;
  next_component_bit++;

  return current_bit;
}

World::id_bitset
World::componentIDsToBitmask(const std::unordered_set<std::string> &ids) {
  id_bitset bitmask;

  // Technically it's O(n), but it's cheap as hell
  for (const auto &id : ids) {
    bitmask |= id_bitset(1) << componentIDToBit(id);
  }

  return bitmask;
}

void World::addEntity(const std::string &id, std::shared_ptr<Entity> entity) {
  if (entities.find(id) != entities.end()) {
    MT_LOG_ERROR("Trying to add existing entity to world by id {}, check for "
                 "id collisions, not daring to overwrite it",
                 id);
    return;
  }

  std::unordered_set<std::string> components_ids = entity->getComponentIDs();
  id_bitset bitmask = componentIDsToBitmask(entity->getComponentIDs());

  MT_LOG_DEBUG("Adding entity {}", id);
  entities[id] = std::pair(entity, bitmask);
  updateCache(id, 0, bitmask);
}

void World::removeEntity(const std::string &id) {
  auto entity = entities.find(id);

  if (entity == entities.end()) {
    MT_LOG_ERROR("Trying to remove non-exising entity by id {}", id);
    return;
  }

  MT_LOG_DEBUG("Removing entity {}", id);
  updateCache(id, entity->second.second, 0);
  entities.erase(id);
}

std::shared_ptr<Entity> World::getEntity(const std::string &id) const {
  auto entity = entities.find(id);

  if (entity == entities.end()) {
    MT_LOG_ERROR("Trying to get non-existing entity {}", id);
    return nullptr;
  }

  return entity->second.first;
}

std::vector<std::pair<std::shared_ptr<Entity>, std::string>>
World::query(const std::unordered_set<std::string> &ids, bool _and) {
  std::vector<std::pair<std::shared_ptr<Entity>, std::string>> result;
  id_bitset bitmask = componentIDsToBitmask(ids);

  if (bitmask == 0) {
    MT_LOG_ERROR("Empty query, returning empty list");
    return {};
  }

  if (_and) { // AND query
    auto cache_hit = and_query_cache.find(bitmask);
    if (cache_hit != and_query_cache.end()) {
      return and_query_cache[bitmask];
    }

    for (const auto &[id, value] : entities) {
      if ((value.second & bitmask) == bitmask) {
        result.push_back(std::pair(value.first, id));
      }
    }

    and_query_cache[bitmask] = result;

  } else { // OR query
    auto cache_hit = or_query_cache.find(bitmask);
    if (cache_hit != or_query_cache.end()) {
      return or_query_cache[bitmask];
    }

    for (const auto &[id, value] : entities) {
      if ((value.second & bitmask) != 0) {
        result.push_back(std::pair(value.first, id));
      }
    }

    or_query_cache[bitmask] = result;
  }

  return result;
}

void World::addComponent(const std::string &id,
                         std::shared_ptr<Component> component) {
  auto entity = getEntity(id);

  if (entity == nullptr) {
    MT_LOG_ERROR("Trying to assign component to non-existing entity {}", id);
    return;
  }

  std::string component_id = component->getComponentID();

  id_bitset old = componentIDsToBitmask(entity->getComponentIDs());

  MT_LOG_DEBUG("Adding component {} to entity {}", component_id, id);
  entity->addComponent(component);
  entities[id].second |= id_bitset(1)
                         << componentIDToBit(component->getComponentID());

  updateCache(id, old, componentIDsToBitmask(entity->getComponentIDs()));
}

void World::removeComponent(const std::string &entity_id,
                            const std::string &component_id) {
  auto entity = getEntity(entity_id);

  if (entity == nullptr) {
    MT_LOG_ERROR("Trying to remove component from non-existing entity {}",
                 entity_id);
    return;
  }

  if (!entity->contains(component_id)) {
    MT_LOG_ERROR("Trying to remove non-existing component {} from entity {}",
                 component_id, entity_id);
    return;
  }

  id_bitset old = componentIDsToBitmask(entity->getComponentIDs());

  MT_LOG_DEBUG("Removing component {} from entity {}", component_id, entity_id);
  entity->removeComponent(component_id);

  // Sets according bit in bitmask to 0
  entities[entity_id].second &=
      ~(id_bitset(1) << componentIDToBit(component_id));

  updateCache(entity_id, old, entities[entity_id].second);
}

void World::updateCache(const std::string &entity_id, id_bitset old_bitmask,
                        id_bitset new_bitmask) {
  /*
   * This method doesn't care about what exists and what doesn't besides
   * simple entity id check, and since it isn't exposed to any public interface
   * I assume that I will use it correctly
   */

  MT_LOG_DEBUG("Called cache update for entity {}, old: ...{}, new: ...{}",
               entity_id, old_bitmask.to_string().erase(0, 1000),
               new_bitmask.to_string().erase(0, 1000));

  auto entity = getEntity(entity_id);

  if (entity == nullptr) {
    MT_LOG_ERROR("Trying to update cache on non-existing entity {}", entity_id);
    return;
  }

  id_bitset shared = old_bitmask & new_bitmask;

  // Shared bits are guaranteed to be present in both masks, thus allowing to
  // xor, getting the difference between them
  id_bitset addition = new_bitmask ^ shared;
  id_bitset subtraction = old_bitmask ^ shared;

  /*
   * old:         00010101
   * new:         01010100
   * shared:      00010100
   * addition:    01000000
   * subtraction: 00000001
   */

  if (subtraction != 0) {
    // AND
    for (auto &[bitmask, cache_entities] : and_query_cache) {
      MT_LOG_DEBUG("Checking AND cache for erasing with bitmask ...{}",
                   bitmask.to_string().erase(0, 1000));

      if (((bitmask & shared) != bitmask) ^
          ((bitmask & old_bitmask) == bitmask)) {
        MT_LOG_DEBUG("Skipping");
        continue;
      }

      for (size_t i = 0; i < cache_entities.size(); i++) {
        MT_LOG_DEBUG("Comparing ids {} and {}", cache_entities[i].second,
                     entity_id);
        if (cache_entities[i].second == entity_id) {
          MT_LOG_DEBUG("Erasing entity {} from AND cache entry",
                       cache_entities[i].second);
          cache_entities.erase(cache_entities.begin() + i);
          break;
        }
      }
    }

    // OR
    for (auto &[bitmask, cache_entities] : or_query_cache) {
      MT_LOG_DEBUG("Checking OR cache for erasing with bitmask ...{}",
                   bitmask.to_string().erase(0, 1000));

      if (((bitmask & shared) == 0) ^ ((bitmask & old_bitmask) != 0)) {
        continue;
      }

      for (size_t i = 0; i < cache_entities.size(); i++) {
        if (cache_entities[i].second == entity_id) {
          MT_LOG_DEBUG("Erasing entity {} from OR cache entry",
                       cache_entities[i].second);
          cache_entities.erase(cache_entities.begin() + i);
          break;
        }
      }
    }
  }

  if (addition != 0) {
    // AND
    for (auto &[bitmask, cache_entities] : and_query_cache) {
      MT_LOG_DEBUG("Checking AND cache for pushing with bitmask ...{}",
                   bitmask.to_string().erase(0, 1000));

      if (!(((bitmask & shared) != bitmask) &&
            ((bitmask & new_bitmask) == bitmask))) {
        continue;
      }

      MT_LOG_DEBUG("Adding entity {} to AND cache entry", entity_id);
      cache_entities.push_back(std::pair(entity, entity_id));
    }

    // OR
    for (auto &[bitmask, cache_entities] : or_query_cache) {
      MT_LOG_DEBUG("Checking OR cache for pushing with bitmask ...{}",
                   bitmask.to_string().erase(0, 1000));

      if (!(((bitmask & shared) == 0) && ((bitmask & new_bitmask) != 0))) {
        continue;
      }

      MT_LOG_DEBUG("Adding entity {} to OR cache entry", entity_id);
      cache_entities.push_back(std::pair(entity, entity_id));
    }
  }
}

} // namespace engine::ecs
