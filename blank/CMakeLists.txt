# inspired by https://github.com/JoeyDeVries/LearnOpenGL/blob/master/CMakeLists.txt
cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(Blank)

set(SOURCES src/main.cpp)

if(UNIX)
    add_compile_options("-Wall" "-std=c++11")
    set(EXTRA_LIBS dl)
endif(UNIX)

# GLFW3
include_directories(../glfw/include)

# GLAD
include_directories(${GLAD_INCLUDE_DIRS})

# GLM
include_directories(../glm)

add_executable(blank ${SOURCES})
target_link_libraries(blank glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${EXTRA_LIBS})
