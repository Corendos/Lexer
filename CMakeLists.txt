cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(lexer)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message("Release build type")
    add_compile_options(-std=c++17 -O3)
else(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message("Debug build type")
    add_compile_options(-std=c++17)
endif(${CMAKE_BUILD_TYPE} STREQUAL "Release")

add_subdirectory(src)