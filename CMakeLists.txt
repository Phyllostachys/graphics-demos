# inspired by https://github.com/JoeyDeVries/LearnOpenGL/blob/master/CMakeLists.txt
cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(RayTracingScreensaver)

set(OLD_SOURCES main_old.cpp)
set(SOURCES main.cpp)

if(WIN32)
    include_directories("include/")
    link_directories("lib/" "lib/glfw-lib-vc2010/")
    set(LIBS glfw3 opengl32 glew32s)
endif(WIN32)

add_executable(ray_tracing_screensaver ${SOURCES})
target_link_libraries(ray_tracing_screensaver ${LIBS})

add_executable(ray_tracing_screensaver_old ${OLD_SOURCES})
target_link_libraries(ray_tracing_screensaver_old ${LIBS})
