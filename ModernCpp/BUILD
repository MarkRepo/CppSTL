load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "gadget",
    hdrs = ["gadget.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "widget",
    hdrs = [
        "widget.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":gadget",
    ],
)

cc_test(
    name = "typeindex",
    srcs = [
        "typeindex_test.cc",
    ],
    deps = [
        ":widget",
        "@gtest",
        "@gtest//:gtest_main",
        "@local_boost//:libboost",
    ],
)

cc_test(
    name = "condvar_test",
    srcs = [
        "condvar_test.cc",
    ],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
    ],
)