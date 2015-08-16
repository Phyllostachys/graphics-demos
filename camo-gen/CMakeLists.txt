cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(camo-generator)

include_directories(src)

if(UNIX)
    add_compile_options("-Wall" "-std=c++11")
    set(EXTRA_LIBS dl)
endif(UNIX)

add_executable(camo-generator src/main.cpp)
