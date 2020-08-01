load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "hello_test",
    srcs = [
        "hello_test.cc",
    ],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",  # Only if hello_test.cc has no main()
    ],
)
