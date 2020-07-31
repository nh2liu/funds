cc_library(
    name = "main",
    srcs = glob(
        ["googletest/src/*.cc",
         "googletest/src/*.h",
         "googletest/include/gtest/internal/**/*.h",
         "googletest/include/gtest/internal/*.h",
        ],
        exclude = ["googletest/src/gtest-all.cc"]
    ),
    hdrs = glob(["googletest/include/gtest/*.h"]),
    copts = ["-Iexternal/gtest/googletest/"],
    includes = [
        "googletest/include",
    ],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)