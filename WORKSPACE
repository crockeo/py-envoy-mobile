workspace(name = "py_envoy_mobile")

# load("@bazel_tools//tools/build_defs/repo:.bzl", "git_repository")

local_repository(
    name = "envoy_mobile",
    path = "envoy-mobile",
)

load("@envoy_mobile//bazel:envoy_mobile_repositories.bzl", "envoy_mobile_repositories")
envoy_mobile_repositories()

local_repository(
    name = "envoy",
    path = "envoy-mobile/envoy",
)

local_repository(
    name = "envoy_build_config",
    path = "envoy-mobile/envoy_build_config",
)

load("@envoy//bazel:api_binding.bzl", "envoy_api_binding")
envoy_api_binding()

load("@envoy//bazel:api_repositories.bzl", "envoy_api_dependencies")
envoy_api_dependencies()

load("@envoy//bazel:repositories.bzl", "envoy_dependencies")
envoy_dependencies()

load("@envoy//bazel:repositories_extra.bzl", "envoy_dependencies_extra")
envoy_dependencies_extra()

load("@envoy//bazel:dependency_imports.bzl", "envoy_dependency_imports")
envoy_dependency_imports()

load("@envoy_mobile//bazel:envoy_mobile_swift_bazel_support.bzl", "swift_support")
swift_support()

load("@envoy_mobile//bazel:envoy_mobile_dependencies.bzl", "envoy_mobile_dependencies")
envoy_mobile_dependencies()

load("@envoy_mobile//bazel:envoy_mobile_toolchains.bzl", "envoy_mobile_toolchains")
envoy_mobile_toolchains()

# providing access to Python.h
# TODO: make this cross-platform
new_local_repository(
    name = "python",
    path = "/usr/local/opt/python3/Frameworks/Python.framework",
    build_file_content = """
cc_library(
    name = "python-lib",
    visibility = ["//visibility:public"],
    srcs = [],
    hdrs = glob(["Headers/**/*.h"]),
    includes = ["Headers"],
)
"""
)