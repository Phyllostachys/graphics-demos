cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(graphics-demos)

# GLFW3
add_subdirectory(glfw)

# GLM
add_subdirectory(glm)

# Utility library
#add_subdirectory(utils)

# blank project
#add_subdirectory(blank)

# camo generator
#add_subdirectory(camo-gen)



if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
    if(UNIX)
        set(EXTRA_LIBS dl)
    endif(UNIX)
endif(UNIX OR MINGW)

set(GLAD_SOURCE src/glad/src/glad.c)

include_directories(glm)
include_directories(glfw/include)
include_directories(src)
include_directories(src/glad/include)

add_executable(raytracing_frag_shader_single src/raytracing_frag_shader_one_sphere.cpp ${GLAD_SOURCE})
target_link_libraries(raytracing_frag_shader_single glfw3 ${GLFW_LIBRARIES} ${EXTRA_LIBS})

add_executable(raytracing_frag_shader_multi src/raytracing_frag_shader_many_spheres.cpp ${GLAD_SOURCE})
target_link_libraries(raytracing_frag_shader_multi glfw3 ${GLFW_LIBRARIES} ${EXTRA_LIBS})

#add_executable(tex src/texturing_tri_demo.cpp src/lodepng.cpp)
#target_link_libraries(tex glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} ${EXTRA_LIBS} shader)
