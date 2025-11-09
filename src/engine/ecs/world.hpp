#ifndef WORLD_H_
#define WORLD_H_

#include "entity.hpp"

#include <bitset>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace engine::ecs {

// In ideal world before release you should count all the components and set
// bitset_size to amount of them
class World {
private:
  // If somehow you exceed 1024 component types I'm impressed, but just make it
  // bigger (if it's not another power of 2 you don't belong to this party)
  const static size_t bitset_size = 1024;
  typedef std::bitset<bitset_size> id_bitset; // I will eat your RAM

  // I know it looks scary, but it just maps entity id to pair of entity and
  // component id bitmask
  std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, id_bitset>>
      entities;

  // Contains bit shift at which you can set according bit for component id to
  // speed up lookups, exists to make it actually persistent
  std::unordered_map<std::string, size_t> component_id_to_bit;
  size_t next_component_bit;

  // Querying is expected to be pretty expensive, but repetitive
  std::unordered_map<
      id_bitset, std::vector<std::pair<std::shared_ptr<Entity>, std::string>>>
      and_query_cache;
  std::unordered_map<
      id_bitset, std::vector<std::pair<std::shared_ptr<Entity>, std::string>>>
      or_query_cache;

  // Registers new id and returns according bitshift
  size_t componentIDToBit(const std::string &id);
  id_bitset componentIDsToBitmask(const std::unordered_set<std::string> &ids);

  // Updates cache involving this speficific entity's components based on
  // difference between bitmasks
  void updateCache(const std::string &entity_id, id_bitset old_bitmask,
                   id_bitset new_bitmask);

public:
  World();
  ~World() = default;

  void addEntity(const std::string &id, std::shared_ptr<Entity> entity);
  void removeEntity(const std::string &id);

  void clear();

  // Returns nullptr if it cannot find entity by specified id
  std::shared_ptr<Entity> getEntity(const std::string &id) const;

  void addComponent(const std::string &id,
                    std::shared_ptr<Component> component);

  void removeComponent(const std::string &entity_id,
                       const std::string &component_id);

  // _and defines if entity should have one of components to be included or all
  // of them, should run in O(n) on first query and O(1) on every next until
  // entity vector has updated
  std::vector<std::pair<std::shared_ptr<Entity>, std::string>>
  query(const std::unordered_set<std::string> &ids, bool _and = true);
};

} // namespace engine::ecs

#endif // WORLD_H_
