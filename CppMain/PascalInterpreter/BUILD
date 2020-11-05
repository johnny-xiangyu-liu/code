load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

cc_library(
    name = "lexer",
    hdrs = ["lexer.h"],
    srcs = ["lexer.cc"],
    deps = [
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/status:status",
        "@com_google_absl//absl/status:statusor",
        ":token",
    ],)

cc_library(
    name = "token",
    hdrs = ["token.h"],
    srcs = ["token.cc"],
    deps = [
         "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "util",
    hdrs = ["util.h"],
    deps = [
         "@com_google_absl//absl/strings",
    ],
)

cc_binary(
    name = "app",
    srcs = ["app.cc"],
    deps = [":lexer"],
)

cc_test(
    name = "lexer_test",
    srcs = ["lexer_test.cc"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":lexer",
        ":token",
        "@com_google_googletest//:gtest_main",
        "@com_google_absl//absl/status",
    ],
)