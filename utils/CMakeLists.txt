cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(utils)

include_directories(include)
include_directories(${GLAD_INCLUDE_DIRS})

add_library(shader STATIC src/shader.cpp)
target_link_libraries(shader glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${EXTRA_LIBS})

set(UTILS_INCLUDE_DIRS include)
