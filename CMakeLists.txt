# inspired by https://github.com/JoeyDeVries/LearnOpenGL/blob/master/CMakeLists.txt
cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(RayTracingScreensaver)

# find the required packages
#find_package(GLM REQUIRED)
#message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
#find_package(GLFW3 REQUIRED)
#message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
#find_package(GLEW REQUIRED)
#message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")

if(WIN32)
    include_directories("include/")
    link_directories("lib/")
    link_directories("lib/glfw-lib-vc2010/")
    set(LIBS glfw3 opengl32 glew32s)
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

add_executable(ray_tracing_screensaver main.cpp)
add_executable(ray_tracing_screensaver_old main_old.cpp)
target_link_libraries(ray_tracing_screensaver_old ${LIBS})
