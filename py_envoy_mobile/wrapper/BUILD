cc_library(
    name = "py_envoy_data",
    srcs = [
        "py_envoy_data.cc",
    ],
    hdrs = [
        "py_envoy_data.h",
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
        ":py_envoy_data",

        "@python//:python-lib",
        "@envoy_mobile//library/common:envoy_main_interface_lib",
    ],
)
