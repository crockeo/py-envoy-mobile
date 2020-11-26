#include "py_envoy_data.h"

#include "library/common/main_interface.h"


Data::Data(const std::string& str) {
  this->data.resize(str.size());
  memcpy(&this->data[0], &str[0], str.size());
}

envoy_data Data::as_envoy_data() {
  return copy_envoy_data(this->data.size(), &this->data[0]);
}
