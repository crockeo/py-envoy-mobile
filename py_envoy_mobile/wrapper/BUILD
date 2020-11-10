cc_binary(
    name = "c_types_wrapper.so",
    visibility = ["//visibility:public"],
    linkshared = True,
    srcs = [
        "c_types_wrapper.cc",
    ],
    deps = [
        "@python//:python-lib",
        "@envoy_mobile//library/common:envoy_main_interface_lib",
    ],
)
