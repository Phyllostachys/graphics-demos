#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include "GL/gl3w.h"
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "shader_util.h"

uint8_t board[600][800];
bool animate = false;

static const char* ppm_header = {
    "P3\n"
    "800 600\n"
    "255\n"
};
#define PPM_BLACK_PIXEL "0 0 0 "
#define PPM_GREEN_PIXEL "107 142 35 "
#define PPM_BROWN_PIXEL "135 74 43 "
enum {
    pxl_black = 0,
    pxl_green,
    pxl_brown
};

static const GLfloat background[] = { 0.0f, 0.0f, 0.0f, 1.0f };

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(int argc, char* argv[])
{
    // init data
    srand(time(NULL));

    int randColor;
    std::ofstream ofs;
    ofs.open("initial_colors.ppm", std::ofstream::out | std::ofstream::app);
    ofs << ppm_header;

    // generate data
    for(int y = 600; y > 0; y--) {
        for(int x = 800; x > 0; x--) {
            randColor = rand() % 3;
            std::cout << randColor << " ";
            board[y][x] = randColor;
            switch(randColor) {
                case pxl_black:
                    ofs << PPM_BLACK_PIXEL;
                    break;
                case pxl_green:
                    ofs << PPM_GREEN_PIXEL;
                    break;
                case pxl_brown:
                    ofs << PPM_BROWN_PIXEL;
                    break;
                default:
                    break;
            }
        }
        std::cout << std::endl;
        ofs << std::endl;
    }
    ofs.close();

#if 0
    if (animate == true) {
        GLFWwindow* window;

        glfwSetErrorCallback(error_callback);

        if (!glfwInit()) {
            exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        window = glfwCreateWindow(800, 600, "Camo-Generator", NULL, NULL);
        if (!window) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        if (!gl3wInit()) {
            exit(EXIT_FAILURE);
        }

        int major, minor, revision;
        glfwGetVersion(&major, &minor, &revision);
        std::cout << "Using OpenGL " << major << "." << minor << "." << revision << std::endl;
        std::cout << "Is OpenGL 4.3 supported?" << (gl3wIsSupported(4, 3) ? " yes" : " no") << std::endl;

        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, key_callback);

        glViewport(0, 0, 800, 600);

        GLuint renderingProgram = compile_shaders();
        GLuint vertexArrayObject;
        glGenVertexArrays(1, &vertexArrayObject);
        glBindVertexArray(vertexArrayObject);

        while (!glfwWindowShouldClose(window)) {
            // do rendering here
            glClearBufferfv(GL_COLOR, 0, background);

            glPointSize(64.0f);
            glUseProgram(renderingProgram);
            glDrawArrays(GL_POINTS, 0, 1);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &vertexArrayObject);
        glDeleteProgram(renderingProgram);
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_SUCCESS);
    } else {
        //
    }
#endif
}
