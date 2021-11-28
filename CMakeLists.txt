cmake_minimum_required(VERSION 3.22)
project(metadata)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
)

FetchContent_MakeAvailable(googletest)

include_directories(include)
add_subdirectory(src)
add_subdirectory(test)

execute_process(COMMAND bash -c "rm ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json; ln -s ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json" OUTPUT_VARIABLE outVar) 
