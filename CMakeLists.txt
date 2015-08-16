cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(graphics-demos)

# GLFW3
add_subdirectory(glfw)

# GLAD
add_subdirectory(glad)

# GLM
add_subdirectory(glm)

# Utility library
add_subdirectory(utils)

# blank project
add_subdirectory(blank)

# camo generator
add_subdirectory(camo-gen)

# Raytracing screensaver
add_subdirectory(raytracing-screensaver)
