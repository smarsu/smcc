cmake_minimum_required(VERSION 3.5.1)
project(smcc)

add_compile_options(-std=c++11)
add_compile_options(-Wall)
add_compile_options(-Werror)
add_compile_options(-O3)
add_compile_options(-g)
add_compile_options(-Wno-format-security)

include_directories(.)

file(GLOB_RECURSE SRC_LIST FOLLOW_SYMLINKS "*.cc")
message(STATUS "SRC_LIST: ${SRC_LIST}")
add_executable(smcc ${SRC_LIST})
