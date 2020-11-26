#pragma once

#include <string>
#include <vector>

#include "library/common/types/c_types.h"


struct Data {
  Data(const std::string& str);

  // copies the contents of this struct into an envoy_data
  envoy_data as_envoy_data();

  std::vector<uint8_t> data;
};
