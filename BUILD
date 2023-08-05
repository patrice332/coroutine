load("@rules_cc//cc:defs.bzl", "cc_toolchain", "cc_toolchain_suite")
load("//:unix_cc_toolchain_config.bzl", "cc_toolchain_config")

package(default_visibility = ["//visibility:public"])

filegroup(name = "empty")

# Unfiltered compiler flags; these are placed at the end of the command
# line, so take precendence over any user supplied flags through --copts or
# such.
unfiltered_compile_flags = [
    # Do not resolve our symlinked resource prefixes to real paths.
    "-no-canonical-prefixes",
    # Reproducibility
    "-Wno-builtin-macro-redefined",
    "-D__DATE__=\"redacted\"",
    "-D__TIMESTAMP__=\"redacted\"",
    "-D__TIME__=\"redacted\"",
]

target_system_name = "x86_64-unknown-linux-gnu"

# Default compiler flags:
compile_flags = [
    "--target=" + target_system_name,
    # Security
    "-U_FORTIFY_SOURCE",  # https://github.com/google/sanitizers/issues/247
    "-fstack-protector",
    "-fno-omit-frame-pointer",
    # Diagnostics
    "-fcolor-diagnostics",
    "-Wall",
    "-Wthread-safety",
    "-Wself-assign",
]

dbg_compile_flags = [
    "-g",
    "-fstandalone-debug",
]

opt_compile_flags = [
    "-g0",
    "-O2",
    "-D_FORTIFY_SOURCE=1",
    "-DNDEBUG",
    "-ffunction-sections",
    "-fdata-sections",
]

link_flags = [
    "--target=" + target_system_name,
    "-lm",
    "-no-canonical-prefixes",
    "-fuse-ld=lld",
    "-Wl,--build-id=md5",
    "-Wl,--hash-style=gnu",
    "-Wl,-z,relro,-z,now",
    "-l:libc++.a",
    "-l:libc++abi.a",
    "-l:libunwind.a",
    # Compiler runtime features.
    "-rtlib=compiler-rt",
    # To support libunwind.
    "-lpthread",
    "-ldl",
]

cxx_flags = [
    "-std=c++20",
    "-stdlib=libc++",
]

# Similar to link_flags, but placed later in the command line such that
# unused symbols are not stripped.
link_libs = []

opt_link_flags = ["-Wl,--gc-sections"]

cxx_builtin_include_directories = [
    "/include",
    "/usr/include",
    "/usr/local/include",
]

cc_toolchain_config(
    name = "k8_toolchain_config",
    abi_libc_version = "unknown",
    abi_version = "unknown",
    ar = "@linux-x86_64-toolchain//:ar",
    clang = "@linux-x86_64-toolchain//:clang",
    clang_plus_plus = "@linux-x86_64-toolchain//:clang_plus_plus",
    compile_flags = compile_flags,
    compiler = "clang",
    cpu = "k8",
    cxx_builtin_include_directories = cxx_builtin_include_directories,
    cxx_flags = cxx_flags,
    dbg_compile_flags = dbg_compile_flags,
    host_system_name = "linux-x86_64",
    ld = "@linux-x86_64-toolchain//:ld",
    link_flags = link_flags,
    link_libs = link_libs,
    objcopy = "@linux-x86_64-toolchain//:objcopy",
    opt_compile_flags = opt_compile_flags,
    opt_link_flags = opt_link_flags,
    strip = "@linux-x86_64-toolchain//:strip",
    supports_start_end_lib = True,
    target_libc = "unknown",
    target_system_name = "linux-x86_64",
    toolchain_identifier = "k8-toolchain",
    unfiltered_compile_flags = unfiltered_compile_flags,
)

filegroup(
    name = "compiler_files",
    srcs = [
        "@linux-x86_64-toolchain//:bin",
        "@linux-x86_64-toolchain//:include",
    ],
)

filegroup(
    name = "linker_files",
    srcs = [
        "@linux-x86_64-toolchain//:bin",
        "@linux-x86_64-toolchain//:lib",
    ],
)

cc_toolchain(
    name = "k8_toolchain",
    all_files = ":empty",
    compiler_files = ":compiler_files",
    dwp_files = "@linux-x86_64-toolchain//:bin",
    linker_files = ":linker_files",
    objcopy_files = "@linux-x86_64-toolchain//:bin",
    strip_files = "@linux-x86_64-toolchain//:bin",
    supports_param_files = False,
    toolchain_config = ":k8_toolchain_config",
    toolchain_identifier = "k8_toolchain_x86_64",
)

cc_toolchain_suite(
    name = "clang_suite",
    toolchains = {
        "k8": ":k8_toolchain",
    },
)
