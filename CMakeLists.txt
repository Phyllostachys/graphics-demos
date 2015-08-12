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
    link_directories("lib/win64/")
    link_directories("lib/win64/vc2013/")
    set(LIBS glfw3 opengl32 glew32s)
elseif(UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
    find_package(OpenGL REQUIRED)
    add_definitions(${OPENGL_DEFINITIONS})
    find_package(X11 REQUIRED)
    # note that the order is important for setting the libs
    # use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
    set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread GLEW)
else()
  set(LIBS )
endif(WIN32)

add_executable(ray_tracing_screensaver main.cpp)
add_executable(ray_tracing_screensaver_old main_old.cpp)
