load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

cc_test(
    name = "regex_test",
    srcs = ["regex_test.cc"],
    deps = [
        ":output",
        "@gtest//:gtest_main",
    ],
)
