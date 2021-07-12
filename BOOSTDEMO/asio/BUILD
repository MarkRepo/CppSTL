load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "timer_test",
    srcs = [
        "asio_timer_test.cc",
    ],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
        "@local_boost//:libboost",
    ],
)

cc_test(
    name = "strand_test",
    srcs = [
        "asio_strand_test.cc",
    ],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
        "@local_boost//:libboost",
    ],
)

cc_test(
    name = "tcp_server",
    srcs = [
        "tcp_server_test.cc",
    ],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
        "@local_boost//:libboost",
    ],
)

cc_test(
    name = "async_tcp_server",
    srcs = [
        "async_tcp_server_test.cc",
    ],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
        "@local_boost//:libboost",
    ],
)

cc_test(
    name = "tcp_client",
    srcs = [
        "tcp_client_test.cc",
    ],
    deps = [
        "@gtest",
        "@gtest//:gtest_main",
        "@local_boost//:libboost",
    ],
)
