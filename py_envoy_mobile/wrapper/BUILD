load("@pybind11_bazel//:build_defs.bzl", "pybind_library")

pybind_library(
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

pybind_library(
    name = "lib_py",
    visibility = ["//visibility:public"],
    srcs = [
        "py_executor_base.cc",
    ],
    hdrs = [
        "py_executor_base.h",
    ],
    deps = [
        ":lib",
    ],
)

exports_files(["module_definition.cc"])
