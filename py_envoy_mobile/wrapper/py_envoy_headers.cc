#include "py_envoy_headers.h"


Headers::Headers() {}

Headers::Headers(const envoy_headers headers) {
  for (int i = 0; i < headers.length; i++) {
    // TODO(chillen): this is unsafe, find a better way to do this
    std::string key((const char *)(headers.headers[i].key.bytes));
    std::string value((const char *)(headers.headers[i].value.bytes));

    this->headers[key] = value;
  }
}

Headers& Headers::add(const std::string& name, const std::string& value) {
  this->headers[name] = value;
  return *this;
}

envoy_headers Headers::as_envoy_headers() const {
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
