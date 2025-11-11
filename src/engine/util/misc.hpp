#ifndef MISC_H_
#define MISC_H_

#include <string>

namespace engine::util {

std::string camelToKebab(const std::string &name);

// Demangles type name from type_info
std::string demangle(const std::string &name);

} // namespace engine::util

#endif // MISC_H_
