cmake_minimum_required(VERSION 2.6)
project(camo-generator)

include_directories(include src)

set(CMAKE_CXX_FLAGS -std=c++11)
add_executable(camo-generator src/main.cpp)
