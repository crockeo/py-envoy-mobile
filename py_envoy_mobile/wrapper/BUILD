cc_library(
    name = "wrapper",
    srcs = [
        "py_envoy_data.cc",
        "py_envoy_headers.cc",
        "py_envoy_http_callbacks.cc",
    ],
    hdrs = [
        "py_envoy_data.h",
        "py_envoy_headers.h",
        "py_envoy_http_callbacks.h",
    ],
    deps = [
        "@python//:python-lib",
        "@envoy_mobile//library/common:envoy_main_interface_lib",
    ],
)


cc_binary(
    name = "c_types_wrapper.so",
    visibility = ["//visibility:public"],
    linkshared = True,
    srcs = [
        "c_types_wrapper.cc",
    ],
    deps = [
        ":wrapper",
        "@python//:python-lib",
        "@envoy_mobile//library/common:envoy_main_interface_lib",
    ],
)
