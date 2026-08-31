cmake_minimum_required(VERSION 4.1)
project(vector LANGUAGES C)

add_library(vector STATIC src/vector.c include/vector.h)
target_include_directories(vector PUBLIC include)
target_compile_features(vector PUBLIC c_std_11)

add_executable(test src/test_main.c)
target_link_libraries(test PUBLIC vector)
