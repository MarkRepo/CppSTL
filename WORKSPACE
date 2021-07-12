load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "gtest",
    branch = "v1.10.x",
    remote = "https://github.com/google/googletest",
)

new_local_repository(
    name = "local_boost",
    build_file = "@//third_party/boost:BUILD.boost",
    path = "/Users/markfqwu",
)
