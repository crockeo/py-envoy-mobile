load("@pybind11_bazel//:build_defs.bzl", "pybind_library")

cc_library(
    name = "lib",
    visibility = ["//visibility:public"],
    srcs = [
        "py_envoy_data.cc",
        "py_envoy_engine.cc",
        "py_envoy_headers.cc",
        "py_envoy_stream.cc",
    ],
    hdrs = [
        "executor_base.h",
        "py_envoy_data.h",
        "py_envoy_engine.h",
        "py_envoy_headers.h",
        "py_envoy_stream.h",
    ],
    deps = [
        "@envoy_mobile//library/common:envoy_main_interface_lib",
    ],
)

exports_files(["module_definition.cc"])
