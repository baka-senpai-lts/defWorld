#ifndef ENTITY_H_
#define ENTITY_H_

#include "component.hpp"

#include <forward_list>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace engine::ecs {

class Entity {
private:
  std::unordered_map<std::string, std::shared_ptr<Component>> components;

public:
  ~Entity() = default;

  inline void addComponent(std::shared_ptr<Component> component) {
    components[component->getComponentID()] = component;
  };

  template <class T, typename... Args>
  inline void addComponent(Args &&...args) {
    static_assert(std::is_base_of_v<Component, T>,
                  "T must derive from engine::ecs::Component");
    addComponent(std::make_shared<T>(std::forward(args)...));
  };

  inline void removeComponent(const std::string &id) { components.erase(id); };

  template <typename T> inline void removeComponent() {
    static_assert(std::is_base_of_v<ComponentBase<T>, T>,
                  "T must derive from engine::ecs::ComponentBase<T>");
    removeComponent(T::ID());
  };

  inline bool contains(const std::string &id) const {
    return components.count(id);
  }

  template <typename T> inline bool contains() const {
    static_assert(std::is_base_of_v<ComponentBase<T>, T>,
                  "T must derive from engine::ecs::ComponentBase<T>");
    return components.count(T::ID());
  }

  // Returns nullptr if there is no component with such id
  inline std::shared_ptr<Component> getComponent(const std::string &id) const {
    auto component = components.find(id);
    return (component != components.end()) ? component->second : nullptr;
  }

  // Returns nullptr if there is no component with such id
  template <typename T>
  inline std::shared_ptr<T> getComponent(const std::string &id) const {
    auto component = components.find(id);
    return (component != components.end())
               ? std::static_pointer_cast<T>(component->second)
               : nullptr;
  }

  // Returns nullptr if there is no component with such id
  template <typename T> inline std::shared_ptr<T> getComponent() const {
    static_assert(std::is_base_of_v<ComponentBase<T>, T>,
                  "T must derive from engine::ecs::ComponentBase<T>");
    return getComponent<T>(T::ID());
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
