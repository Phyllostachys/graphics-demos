cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(graphics-demos)

# GLFW3
add_subdirectory(glfw)

# GLAD
add_subdirectory(glad)

# GLM
add_subdirectory(glm)

# camo generator
add_subdirectory(camo-gen)

# Raytracing screensaver
add_subdirectory(raytracing-screensaver)
