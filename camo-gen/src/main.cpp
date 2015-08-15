#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <string>

//#include "GL/gl3w.h"
//#define GLFW_INCLUDE_GLU
//#include <GLFW/glfw3.h>

//#include "shader_util.h"

#define stringify(thing) #thing

const uint32_t width = 800, height = 600;
uint8_t buffer1[600][800];
uint8_t buffer2[600][800];
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

enum {
    alg_linear,
    alg_spiral
} algorithm = alg_linear;

//static const GLfloat background[] = { 0.0f, 0.0f, 0.0f, 1.0f };

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

/*
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
*/

static int32_t searchForSimilar(int32_t x, int32_t y, uint8_t color)
{
    int32_t result = 0;

    // top
    if (buffer1[y - 1][x - 1] == color) { result++; }
    if (buffer1[y - 1][x] == color) { result++; }
    if (buffer1[y - 1][x + 1] == color) { result++; }

    // left and right
    if (buffer1[y][x - 1] == color) { result++; }
    if (buffer1[y][x + 1] == color) { result++; }

    // bottom
    if (buffer1[y + 1][x - 1] == color) { result++; }
    if (buffer1[y + 1][x] == color) { result++; }
    if (buffer1[y + 1][x - 1] == color) { result++; }

    return result;
}

static int32_t* getSurroundCounts(int32_t x, int32_t y)
{
    int32_t bl = 0, gr = 0, br = 0;
    int32_t* ret = new int32_t[3];
    ret[0] = searchForSimilar(x, y, pxl_black);
    ret[1] = searchForSimilar(x, y, pxl_green);
    ret[2] = searchForSimilar(x, y, pxl_brown);
    return ret;
}

static uint8_t sampleFanbase(int32_t x, int32_t y, uint8_t color)
{
    int32_t bl = 0, gr = 0, br = 0;
    uint8_t newColor = pxl_black;
    int32_t* surroundings = getSurroundCounts(x,y);

    switch (color) {
        case pxl_black:
            if (surroundings[0] >= 4) {
                newColor = pxl_black;
                break;
            } else {
                if (surroundings[1] == surroundings[2]) {
                    newColor = rand() % 3;
                } else if (surroundings[1] > surroundings[2]) {
                    newColor = pxl_green;
                } else {
                    newColor = pxl_brown;
                }
            }
            break;

        case pxl_green:
            if (surroundings[1] >= 4) {
                newColor = pxl_green;
                break;
            } else {
                if (surroundings[0] == surroundings[2]) {
                    newColor = rand() % 3;
                } else if (surroundings[0] > surroundings[2]) {
                    newColor = pxl_black;
                } else {
                    newColor = pxl_brown;
                }
            }
            break;

        case pxl_brown:
            if (surroundings[2] >= 4) {
                newColor = pxl_brown;
                break;
            } else {
                if (surroundings[1] == surroundings[0]) {
                    newColor = rand() % 3;
                } else if (surroundings[1] > surroundings[0]) {
                    newColor = pxl_green;
                } else {
                    newColor = pxl_black;
                }
            }
            break;

        default:
            break;
    }

    delete [] surroundings;
    return newColor;
}

static void outputPPM(int32_t iteration)
{
    std::ofstream ofs;
    std::string filename;

    if (algorithm == alg_linear) {
        filename += stringify(alg_linear);
    } else if (algorithm == alg_spiral) {
        filename += stringify(alg_spiral);
    }
    filename += "_result_";
    if (iteration > 9) {
        filename += std::to_string(iteration);
    } else {
        filename += "0";
        filename += std::to_string(iteration);
    }
    filename += "_itr.ppm";

    ofs.open(filename, std::ofstream::out | std::ofstream::app);
    ofs << ppm_header;

    for (int32_t y = 0; y < 600; y++) {
        for (int32_t x = 0; x < 800; x++) {
            switch (buffer2[y][x]) {
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
        ofs << std::endl;
    }
    ofs.close();
}

int main(int argc, char* argv[])
{
    // init data
    srand(time(NULL));

    int32_t randColor;
    std::ofstream ofs;
    ofs.open("initial_colors.ppm", std::ofstream::out | std::ofstream::app);
    ofs << ppm_header;

    // init data
    for (int32_t y = 0; y < 600; y++) {
        for (int32_t x = 0; x < 800; x++) {
            randColor = rand() % 3;
            buffer1[y][x] = randColor;
            switch (randColor) {
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
        ofs << std::endl;
    }
    ofs << std::endl;
    ofs.close();

    // do the thing
    int32_t iterations = 0;
    do {
        for (int32_t y = 1; y < height - 1; y++) {
            for (int32_t x = 1; x < width - 1; x++) {
                //int32_t similarFans = searchForSimilar(x, y, buffer1[y][x]);
                //if (similarFans >= 4) { continue; }
                //else {
                    uint8_t newColor = sampleFanbase(x, y, buffer1[y][x]);
                    buffer2[y][x] = newColor;
                //}
            }
        }

        outputPPM(iterations);
        iterations++;

        for (int32_t y = height - 1; y > 1; y--) {
            for (int32_t x = width - 1; x > 1; x--) {
                buffer1[y][x] = buffer2[y][x];
            }
        }
    } while (iterations <= 25);

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
    return 0;
}
