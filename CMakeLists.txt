cmake_minimum_required(VERSION 4.1)
project(vector C)

set(CMAKE_C_STANDARD 11)

add_library(vector STATIC vector.c vector.h)

add_executable(test_main test_main.c)
target_link_libraries(test_main PRIVATE vector)