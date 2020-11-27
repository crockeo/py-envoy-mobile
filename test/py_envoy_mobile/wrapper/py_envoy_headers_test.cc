#include "py_envoy_mobile/wrapper/py_envoy_headers.h"

#include "gtest/gtest.h"
#include "library/common/types/c_types.h"

TEST(TestHeaders, TestHeadersEmpty) {
  Headers headers;
  EXPECT_EQ(headers.headers.size(), 0);
}

TEST(TestHeaders, TestHeadersFromEnvoyHeaders) {
  envoy_headers raw_headers;
  raw_headers.length = 2;
  raw_headers.headers = (envoy_header *)safe_malloc(2 * sizeof(envoy_header));

  {
    const char *key = "key";
    const char *value = "value";

    raw_headers.headers[0] = envoy_header {
      .key = copy_envoy_data(strlen(key), (const uint8_t *)key),
      .value = copy_envoy_data(strlen(value), (const uint8_t *)value),
    };
  }

  {
    const char *key = "key2";
    const char *value = "value2";

    raw_headers.headers[1] = envoy_header {
      .key = copy_envoy_data(strlen(key), (const uint8_t *)key),
      .value = copy_envoy_data(strlen(value), (const uint8_t *)value),
    };
  }

  Headers headers(raw_headers);

  ASSERT_EQ(headers.headers.size(), 2);
  EXPECT_EQ(headers.headers["key"], "value");
  EXPECT_EQ(headers.headers["key2"], "value2");

  release_envoy_headers(raw_headers);
}

TEST(TestHeaders, TestHeadersAdd) {
  Headers headers;

  EXPECT_EQ(headers.headers.size(), 0);

  headers.add("key", "value").add("key2", "value2");

  EXPECT_EQ(headers.headers.size(), 2);
  EXPECT_EQ(headers.headers["key"], "value");
  EXPECT_EQ(headers.headers["key2"], "value2");
}

TEST(TestHeaders, TestHeadersAsEnvoyHeaders) {
  // Headers headers;
  // headers
  //   .add("Content-Type", "application/json")
  //   .add("Accept", "application/json");

  // envoy_headers raw_headers = headers.as_envoy_headers();
  // release_envoy_headers(raw_headers);
}
