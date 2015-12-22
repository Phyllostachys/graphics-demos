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
add_subdirectory(utils)

# blank project
#add_subdirectory(blank)

# camo generator
add_subdirectory(camo-gen)

# Raytracing screensaver
add_subdirectory(raytracing-screensaver)

# Texture test
add_subdirectory(texturing-test)
