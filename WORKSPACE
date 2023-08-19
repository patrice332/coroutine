workspace(name = "com_github_patrice332_coroutine")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

local_repository(
    name = "com_github_patrice332_bazel_toolchains",
    path = "build/toolchain",
)

load("@com_github_patrice332_bazel_toolchains//:repo.bzl", "init_toolchains")

init_toolchains()

http_archive(
    name = "bazel_skylib",
    sha256 = "66ffd9315665bfaafc96b52278f57c7e2dd09f5ede279ea6d39b2be471e7e3aa",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.4.2/bazel-skylib-1.4.2.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.4.2/bazel-skylib-1.4.2.tar.gz",
    ],
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()

http_archive(
    name = "com_google_absl",
    sha256 = "5366d7e7fa7ba0d915014d387b66d0d002c03236448e1ba9ef98122c13b35c36",
    strip_prefix = "abseil-cpp-20230125.3",
    urls = ["https://github.com/abseil/abseil-cpp/archive/refs/tags/20230125.3.tar.gz"],
)

http_archive(
    name = "com_google_googletest",
    sha256 = "ad7fdba11ea011c1d925b3289cf4af2c66a352e18d4c7264392fead75e919363",
    strip_prefix = "googletest-1.13.0",
    urls = ["https://github.com/google/googletest/archive/refs/tags/v1.13.0.tar.gz"],
)

http_archive(
    name = "com_googlesource_code_re2",
    sha256 = "18cf85922e27fad3ed9c96a27733037da445f35eb1a2744c306a37c6d11e95c4",
    strip_prefix = "re2-2023-07-01",
    urls = ["https://github.com/google/re2/archive/refs/tags/2023-07-01.tar.gz"],
)

http_archive(
    name = "rules_cc",
    sha256 = "ae46b722a8b8e9b62170f83bfb040cbf12adb732144e689985a66b26410a7d6f",
    strip_prefix = "rules_cc-0.0.8",
    urls = ["https://github.com/bazelbuild/rules_cc/releases/download/0.0.8/rules_cc-0.0.8.tar.gz"],
)

http_archive(
    name = "rules_foreign_cc",
    sha256 = "2a4d07cd64b0719b39a7c12218a3e507672b82a97b98c6a89d38565894cf7c51",
    strip_prefix = "rules_foreign_cc-0.9.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/refs/tags/0.9.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://bazelbuild.github.io/rules_foreign_cc/0.9.0/flatten.html#rules_foreign_cc_dependencies
rules_foreign_cc_dependencies()

http_archive(
    name = "com_github_libuv",
    build_file = "//third_party:libuv.BUILD",
    sha256 = "458e34d5ef7f3c0394a2bfd8c39d757cb1553baa5959b9b4b45df63aa027a228",
    strip_prefix = "libuv-1.45.0",
    urls = ["https://github.com/libuv/libuv/archive/refs/tags/v1.45.0.tar.gz"],
)
