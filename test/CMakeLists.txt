enable_testing()

add_executable(metadata-test test.cpp)

target_link_libraries(metadata-test directory gtest_main)

include(GoogleTest)
gtest_discover_tests(metadata-test)
