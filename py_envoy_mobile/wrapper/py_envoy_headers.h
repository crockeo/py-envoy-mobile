#pragma once

#include <unordered_map>
#include <string>

#include "library/common/types/c_types.h"


// Headers wraps the envoy_headers type. It prefers not to actually maintain an envoy_headers, but
// rather translate between that representation and a safer, managed C++ representation.
struct Headers {
  Headers();
  Headers(const envoy_headers headers);

  Headers& add(const std::string& name, const std::string& value);

  envoy_headers as_envoy_headers() const;

  std::unordered_map<std::string, std::string> headers;
};
