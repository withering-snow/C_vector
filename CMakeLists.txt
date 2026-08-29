cmake_minimum_required(VERSION 4.1)
project(vector C)

set(CMAKE_C_STANDARD 11)

add_library(vector STATIC src/vector.c)
target_include_directories(vector PUBLIC include)

add_executable(test src/test_main.c)
target_link_libraries(test vector)