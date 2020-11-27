#include "py_envoy_data.h"

#include <cstdlib>

#include "library/common/main_interface.h"


Data::Data(const std::string& str) {
  this->data.resize(str.size());
  memcpy(&this->data[0], &str[0], str.size());
}

Data::Data(const envoy_data data) {
  this->data.resize(data.length);
  memcpy(&this->data[0], data.bytes, data.length);
}

envoy_data Data::as_envoy_data() const {
  return copy_envoy_data(this->data.size(), &this->data[0]);
}
