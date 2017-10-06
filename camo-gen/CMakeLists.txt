cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(camo-generator)

#include_directories("C:/Users/jacob.shaffer/.local/include")
include_directories(src)

find_package(SDL2)

add_executable(camo-generator src/main.cpp src/texture.cpp)
target_link_libraries(camo-generator SDL2)
#target_link_libraries(camo-generator C:/Users/jacob.shaffer/.local/lib/SDL2.lib)
