"Initializes the toolchains"

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def init_toolchains():
    if "linux-x86_64-toolchain" not in native.existing_rules():
        http_archive(
            name = "linux-x86_64-toolchain",
            url = "https://github.com/patrice332/bazel-toolchains/releases/download/clang-16.0.6/clang-16.0.6.linux-x86_64.tar.xz",
            build_file = "@com_github_patrice332_bazel_toolchains//:toolchain.BUILD",
            sha256 = "1ac219063fe4a098930ff98fa6900d77c17cfc01bdfd52c8dbb417572a2fcef2",
            strip_prefix = "clang-16.0.6",
        )
