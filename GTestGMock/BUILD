load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_test(
    name = "gtest_test",
    srcs = [
        "gtest_test.cc",
    ],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "gmock_test",
    srcs = [
        "gmock_test.cpp",
    ],
    deps = [
        ":turtle",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "turtle",
    hdrs = ["turtle.h"],
    visibility = ["//visibility:public"],
)
