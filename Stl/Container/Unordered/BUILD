load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "hash_set_test",
    srcs = ["unorder_set_test.cc"],
    deps = [
        "//Stl/Container:common",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "hash_map_test",
    srcs = ["unorder_map_test.cc"],
    deps = [
        "//Stl/Container:common",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "hash_test",
    srcs = ["hash_test.cc"],
    deps = [
        "//Stl/Container:common",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
