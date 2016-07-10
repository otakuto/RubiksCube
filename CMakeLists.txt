cmake_minimum_required(VERSION 3.3.1)
project(RubiksCube CXX)
add_executable(RubiksCube main.cpp)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-Wall -std=c++1z -stdlib=libc++")

target_link_libraries(RubiksCube glfw GL GLU)
