#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <string>

namespace engine::ecs {

// Purely generic, exists to store components together, inherit before using
class Component {
protected:
  // Technically you can turn this into template to support any type for ids,
  // but why
  std::string component_id;

public:
  Component() : component_id("replace-me") {}
  virtual ~Component() = default;

  inline const std::string &getComponentID() const {
    return this->component_id;
  }
};

} // namespace engine::ecs

#endif // COMPONENT_H_
