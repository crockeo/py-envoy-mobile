cc_binary(
    name = "envoy_mobile_wrapper.so",
    visibility = ["//visibility:public"],
    linkshared = True,
    srcs = [
        "c_types_wrapper.cc",
    ],
    deps = [
        "@python//:python-lib",
    ],
    # deps = [
    #     "@envoy_mobile//library/common:envoy_main_interface_lib"
    # ],
)
