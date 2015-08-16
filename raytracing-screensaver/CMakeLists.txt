cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(RayTracingScreensaver)

set(SINGLE_SOURCES main_single.cpp)
set(MULTI_SOURCES main_multi.cpp)

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

add_executable(raytrace_srcnsvr_single ${SINGLE_SOURCES})
target_link_libraries(raytrace_srcnsvr_single glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${EXTRA_LIBS})

add_executable(raytrace_srcnsvr_multi ${MULTI_SOURCES})
target_link_libraries(raytrace_srcnsvr_multi glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${EXTRA_LIBS})
