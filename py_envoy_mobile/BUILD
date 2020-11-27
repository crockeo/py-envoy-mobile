load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

py_binary(
    name = "py_envoy_mobile",
    srcs = [
        ":__init__.py",
    ],
    main = "__init__.py",
    data = [
        ":wrapper.so",
    ],
    python_version = "PY3",
)

pybind_extension(
    name = "wrapper",
    visibility = ["//visibility:public"],
    srcs = [
        "//py_envoy_mobile/wrapper:module_definition.cc",
    ],
    deps = [
        "//py_envoy_mobile/wrapper:lib",
        "@envoy_mobile//library/common:envoy_main_interface_lib",
    ],
)


sh_binary(
    name = "debug",
    srcs = ["run_debug.sh"],
    data = [":py_envoy_mobile"],
)
