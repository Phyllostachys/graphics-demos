# inspired by https://github.com/JoeyDeVries/LearnOpenGL/blob/master/CMakeLists.txt
cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(RayTracingScreensaver)

set(OLD_SOURCES main_old.cpp)
set(SOURCES main.cpp)

if(WIN32)
    include_directories("include/")
    link_directories("lib/" "lib/glfw-lib-vc2010/")
    set(LIBS glfw3dll opengl32 glew32s)
elseif(UNIX)
    add_compile_options("-Wall" "-Werror" "-std=c++11")

    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})

    find_library(GLEW_LIBRARY GLEW)

    # note that the order is important for setting the libs
    # use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
    set(LIBS ${GLFW_STATIC_LIBRARIES} dl pthread GLEW)
else()
  set(LIBS )
endif(WIN32)

add_executable(ray_tracing_screensaver ${SOURCES})
target_link_libraries(ray_tracing_screensaver ${LIBS})

add_executable(ray_tracing_screensaver_old ${OLD_SOURCES})
target_link_libraries(ray_tracing_screensaver_old ${LIBS})
