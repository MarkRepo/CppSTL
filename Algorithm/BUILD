load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "helper",
    hdrs = ["algo_helper.h"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "algo_test",
    srcs = ["algo_test.cc"],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "nonmodify_test",
    srcs = ["nonmodifying_test.cc"],
    deps = [
        ":helper",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
cc_test(
    name = "modify_test",
    srcs = ["modifying_test.cc"],
    deps = [
        ":helper",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

