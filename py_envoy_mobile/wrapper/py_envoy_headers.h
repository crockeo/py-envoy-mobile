#pragma once

#include <unordered_map>
#include <string>

#include "library/common/types/c_types.h"


// Headers wraps the envoy_headers type. It prefers not to actually maintain an envoy_headers, but
// rather translate between that representation and a safer, managed C++ representation.
struct Headers {
  Headers();
  Headers(const envoy_headers headers);


  // TODO: implement stuff here that will let me iterate through the headers and retrieve values
  // https://pybind11.readthedocs.io/en/stable/reference.html#_CPPv4NK10object_api5beginEv
  //
  // std::string operator[](const std::string&& key) const);
  // py::iterator begin() const;
  // py::iterator end() const;

  Headers& add(const std::string& name, const std::string& value);

  envoy_headers as_envoy_headers() const;

  std::unordered_map<std::string, std::string> headers;
};
