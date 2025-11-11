#include "misc.hpp"

#include <cxxabi.h>

namespace engine::util {

std::string camelToKebab(const std::string &name) {
  std::string result;
  for (size_t i = 0; i < name.size(); ++i) {
    char c = name[i];
    if (std::isupper(c)) {
      if (i > 0)
        result += '-';
      result += std::tolower(c);
    } else {
      result += c;
    }
  }
  return result;
}

std::string demangle(const std::string &name) {
  int status;
  std::string result = abi::__cxa_demangle(name.c_str(), NULL, NULL, &status);

  if (status != 0) {
    // Something went wrong and I don't feel like dying
    return name;
  }

  return result;
}

} // namespace engine::util
