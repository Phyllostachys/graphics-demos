@echo off

cl /Zi /Wall /wd4054 /wd4055 /wd4201 /wd4255 /wd4464 /wd4710 /wd4820 /I include src\scene_with_camera.cpp src\shader.cpp src\glad.c glfw3dll.lib
