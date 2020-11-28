#include "py_envoy_headers.h"

#include "py_envoy_data.h"


Headers::Headers() {}

Headers::Headers(const envoy_headers headers) {
  for (int i = 0; i < headers.length; i++) {
    Data key(headers.headers[i].key);
    Data value(headers.headers[i].value);

    this->headers[key.as_str()] = value.as_str();
  }
}

std::string Headers::operator[](const std::string&& key) const {
  return this->headers.at(key);
}

HeadersIterator Headers::begin() const {
  return this->headers.cbegin();
}

HeadersIterator Headers::end() const {
  return this->headers.cend();
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
    i++;
  }

  return headers;
}
