load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "stack",
    srcs = [
        "stack.cc",
    ],
    hdrs = [
        "stack.hh",
    ],
    deps = [
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
    ],
)

cc_test(
    name = "stack_test",
    srcs = [
        "stack_test.cc",
    ],
    deps = [
        ":stack",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "task",
    srcs = [
        "task.cc",
    ],
    hdrs = [
        "task.hh",
    ],
    deps = [
        ":stack",
    ],
)

cc_test(
    name = "task_test",
    srcs = [
        "task_test.cc",
    ],
    deps = [
        ":task",
        "@com_google_googletest//:gtest_main",
    ],
)
