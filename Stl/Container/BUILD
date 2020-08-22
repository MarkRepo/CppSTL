load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "common",
    hdrs = ["common.h"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "common_test",
    srcs = ["common_test.cc"],
    deps = [
        ":common",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
