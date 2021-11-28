add_library(directory directory.cpp)

add_executable(metadata main.cpp)

target_link_libraries(metadata directory)
