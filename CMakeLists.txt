cmake_minimum_required(VERSION 4.1)
project(vector C)

set(CMAKE_C_STANDARD 11)

add_library(vector STATIC vector.c vector.h)
target_include_directories(vector PUBLIC .)