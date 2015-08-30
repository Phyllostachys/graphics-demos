cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(camo-generator)

include_directories(src)

if(UNIX OR MINGW)
    add_compile_options("-Wall" "-std=c++11")
    if(UNIX)
        set(EXTRA_LIBS dl)
    endif(UNIX)
endif(UNIX OR MINGW)

add_executable(camo-generator src/main.cpp)
