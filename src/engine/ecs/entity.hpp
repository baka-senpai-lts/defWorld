#ifndef ENTITY_H_
#define ENTITY_H_

#include "component.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace engine::ecs {

class Entity {
private:
  std::unordered_map<std::string, std::shared_ptr<Component>> components;

public:
  ~Entity() = default;

  void operator=(Entity const &entity) {
    // Just copying the components
    components.clear();

    for (const auto &[id, component] : entity.components) {
      components[id] = std::make_shared<Component>(*component);
    }
  };

  inline void addComponent(std::shared_ptr<Component> component) {
    components[component->getComponentID()] = component;
  };

  inline void removeComponent(const std::string &id) { components.erase(id); };

  inline bool contains(const std::string &id) const {
    return components.count(id);
  }

  // Returns nullptr if there is no component with such id
  inline std::shared_ptr<Component> getComponent(const std::string &id) const {
    auto component = components.find(id);
    return (component != components.end()) ? component->second : nullptr;
  }

  inline std::unordered_set<std::string> getComponentIDs() const {
    std::unordered_set<std::string> ids;

    for (const auto &[id, _] : components) {
      ids.insert(id);
    }

    return ids;
  }
};

} // namespace engine::ecs

#endif // ENTITY_H_
