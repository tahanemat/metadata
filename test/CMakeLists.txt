enable_testing()

add_executable(metadata-test test.cpp)

target_link_libraries(metadata-test directory gtest_main)

include(GoogleTest)
gtest_discover_tests(metadata-test)

add_test(metadata-test metadata-test)

set(UNIT_TEST metadata-test)
add_custom_command(
     TARGET ${UNIT_TEST}
     COMMENT "Run tests"
     POST_BUILD 
     COMMAND ${UNIT_TEST} --gtest_color=yes
)
