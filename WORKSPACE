workspace(name = "py_envoy_mobile")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

# pybind11
git_repository(
    name = "pybind11_bazel",
    remote = "git@github.com:pybind/pybind11_bazel",
    commit = "26973c0ff320cb4b39e45bc3e4297b82bc3a6c09",
)
new_git_repository(
    name = "pybind11",
    build_file = "@pybind11_bazel//:pybind11.BUILD",
    remote = "git@github.com:pybind/pybind11",
    commit = "f1abf5d9159b805674197f6bc443592e631c9130",
)
load("@pybind11_bazel//:python_configure.bzl", "python_configure")
python_configure(
    name = "local_config_python",
    python_version = "3",
)

local_repository(
    name = "envoy_mobile",
    path = "envoy-mobile",
)

# envoy + envoy-mobile
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
