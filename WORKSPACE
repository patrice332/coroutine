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
