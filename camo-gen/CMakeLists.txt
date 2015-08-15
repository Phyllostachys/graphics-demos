cmake_minimum_required(VERSION 2.6)
project(camo-generator)

include_directories(include src)

if(UNIX)
    add_compile_options("-Wall" "-std=c++11")
    set(EXTRA_LIBS dl)
endif(UNIX)

add_executable(camo-generator src/main.cpp)
