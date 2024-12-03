add_executable(util_tests
    UtilStr.t.cpp
    UtilFilesystem.t.cpp
)

find_package(GTest REQUIRED)
target_link_libraries(util_tests PRIVATE GTest::gtest_main GTest::gmock)

target_link_libraries(util_tests PRIVATE util)

gtest_discover_tests(util_tests)
