#include "py_envoy_headers.h"


Headers& Headers::add(const std::string& name, const std::string& value) {
  this->headers[name] = value;
  return *this;
}

Headers Headers::from_envoy_headers(envoy_headers headers) {
  // TODO: implement it in this direction
  return Headers();
}

envoy_headers Headers::to_envoy_headers() const {
  envoy_headers headers;

  headers.length = this->headers.size();
  headers.headers = (envoy_header *)safe_malloc(sizeof(envoy_header) * this->headers.size());

  int i = 0;
  for (const auto& pair : this->headers) {
    envoy_data key = copy_envoy_data(pair.first.size(), (const unsigned char *)pair.first.c_str());
    envoy_data value = copy_envoy_data(pair.second.size(), (const unsigned char *)pair.second.c_str());

    headers.headers[i] = envoy_header {
      .key = key,
      .value = value,
    };
  }

  return headers;
}
