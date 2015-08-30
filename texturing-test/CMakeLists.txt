# inspired by https://github.com/JoeyDeVries/LearnOpenGL/blob/master/CMakeLists.txt
cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(Texturing_Test)

set(SOURCES src/main.cpp src/lodepng.cpp src/glad.c)

if(UNIX OR MINGW)
    add_compile_options("-Wall" "-std=c++11")
    if(UNIX)
        set(EXTRA_LIBS dl)
    endif(UNIX)
endif(UNIX OR MINGW)

# GLFW3
include_directories(../glfw/include)

# GLAD
#include_directories(${GLAD_INCLUDE_DIRS})
include_directories(include)

# utils
include_directories(../utils/include)

add_executable(tex ${SOURCES})
target_link_libraries(tex glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${EXTRA_LIBS} shader)
