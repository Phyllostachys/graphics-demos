cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(graphics-demos)

#if(EXISTS ".gitmodules")
#    message(" -- Update git submodules --")
#    execute_process(COMMAND git submodule update --init --recursive)
#endif()

# GLFW3
add_subdirectory(glfw)

# GLAD
add_subdirectory(glad)

# GLM
add_subdirectory(glm)

# Utility library
#add_subdirectory(utils)

# blank project
#add_subdirectory(blank)

# camo generator
#add_subdirectory(camo-gen)



if(UNIX OR MINGW)
    add_compile_options("-Wall" "-std=c++11")
    if(UNIX)
        set(EXTRA_LIBS dl)
    endif(UNIX)
endif(UNIX OR MINGW)

#include_directories(include)
include_directories(glfw/include/)
include_directories(${GLAD_INCLUDE_DIRS})
include_directories(glm)

add_executable(raytracing_frag_shader_single src/raytracing_frag_shader_one_sphere.cpp)
target_link_libraries(raytracing_frag_shader_single glfw3 ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${EXTRA_LIBS})

add_executable(raytracing_frag_shader_multi src/raytracing_frag_shader_many_spheres.cpp)
target_link_libraries(raytracing_frag_shader_single glfw3 ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${EXTRA_LIBS})

#add_executable(tex src/texturing_tri_demo.cpp src/lodepng.cpp)
#target_link_libraries(tex glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${EXTRA_LIBS} shader)
