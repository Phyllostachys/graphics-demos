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
    if (board[y - 1][x - 1] == color) { result++; }
    if (board[y - 1][x] == color) { result++; }
    if (board[y - 1][x + 1] == color) { result++; }

    // left and right
    if (board[y][x - 1] == color) { result++; }
    if (board[y][x + 1] == color) { result++; }

    // bottom
    if (board[y + 1][x - 1] == color) { result++; }
    if (board[y + 1][x] == color) { result++; }
    if (board[y + 1][x - 1] == color) { result++; }

    return result;
}

static uint8_t sampleFanbase(int32_t x, int32_t y, uint8_t color)
{
    int32_t bl = 0, gr = 0, br = 0;
    uint8_t newColor = pxl_black;

    switch (color) {
        case pxl_black:
            // top
            if (board[y - 1][x - 1] == pxl_green)      { gr++; }
            else if (board[y - 1][x - 1] == pxl_brown) { br++; }

            if (board[y - 1][x] == pxl_green)      { gr++; }
            else if (board[y - 1][x] == pxl_brown) { br++; }

            if (board[y - 1][x + 1] == pxl_green)      { gr++; }
            else if (board[y - 1][x + 1] == pxl_brown) { br++; }

            // left and right
            if (board[y][x - 1] == pxl_green)      { gr++; }
            else if (board[y][x - 1] == pxl_brown) { br++; }

            if (board[y][x + 1] == pxl_green)      { gr++; }
            else if (board[y][x + 1] == pxl_brown) { br++; }

            // bottom
            if (board[y + 1][x - 1] == pxl_green)      { gr++; }
            else if (board[y + 1][x - 1] == pxl_brown) { br++; }

            if (board[y + 1][x] == pxl_green)      { gr++; }
            else if (board[y + 1][x] == pxl_brown) { br++; }

            if (board[y + 1][x - 1] == pxl_green)      { gr++; }
            else if (board[y + 1][x - 1] == pxl_brown) { br++; }

            if (gr > br)      { newColor = pxl_green; }
            else if (gr < br) { newColor = pxl_brown; }
            else              { newColor = rand() % 3;  }
            break;

        case pxl_green:
            // top
            if (board[y - 1][x - 1] == pxl_black)      { bl++; }
            else if (board[y - 1][x - 1] == pxl_brown) { br++; }

            if (board[y - 1][x] == pxl_black)      { bl++; }
            else if (board[y - 1][x] == pxl_brown) { br++; }

            if (board[y - 1][x + 1] == pxl_black)      { bl++; }
            else if (board[y - 1][x + 1] == pxl_brown) { br++; }

            // left and right
            if (board[y][x - 1] == pxl_black)      { bl++; }
            else if (board[y][x - 1] == pxl_brown) { br++; }

            if (board[y][x + 1] == pxl_black)      { bl++; }
            else if (board[y][x + 1] == pxl_brown) { br++; }

            // bottom
            if (board[y + 1][x - 1] == pxl_black)      { bl++; }
            else if (board[y + 1][x - 1] == pxl_brown) { br++; }

            if (board[y + 1][x] == pxl_black)      { bl++; }
            else if (board[y + 1][x] == pxl_brown) { br++; }

            if (board[y + 1][x - 1] == pxl_black)      { bl++; }
            else if (board[y + 1][x - 1] == pxl_brown) { br++; }

            if (bl > br)      { newColor = pxl_black; }
            else if (bl < br) { newColor = pxl_brown; }
            else              { newColor = rand() % 3;  }
            break;

        case pxl_brown:
            // top
            if (board[y - 1][x - 1] == pxl_green)      { gr++; }
            else if (board[y - 1][x - 1] == pxl_black) { bl++; }

            if (board[y - 1][x] == pxl_green)      { gr++; }
            else if (board[y - 1][x] == pxl_black) { bl++; }

            if (board[y - 1][x + 1] == pxl_green)      { gr++; }
            else if (board[y - 1][x + 1] == pxl_black) { bl++; }

            // left and right
            if (board[y][x - 1] == pxl_green)      { gr++; }
            else if (board[y][x - 1] == pxl_black) { bl++; }

            if (board[y][x + 1] == pxl_green)      { gr++; }
            else if (board[y][x + 1] == pxl_black) { bl++; }

            // bottom
            if (board[y + 1][x - 1] == pxl_green)      { gr++; }
            else if (board[y + 1][x - 1] == pxl_black) { bl++; }

            if (board[y + 1][x] == pxl_green)      { gr++; }
            else if (board[y + 1][x] == pxl_black) { bl++; }

            if (board[y + 1][x - 1] == pxl_green)      { gr++; }
            else if (board[y + 1][x - 1] == pxl_black) { bl++; }

            if (gr > bl)      { newColor = pxl_green; }
            else if (gr < bl) { newColor = pxl_black; }
            else              { newColor = rand() % 3;  }
            break;

        default:
            break;
    }

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
    filename += std::to_string(iteration);
    filename += "_itr.ppm";

    ofs.open(filename, std::ofstream::out | std::ofstream::app);
    ofs << ppm_header;

    for (int32_t y = 0; y < 600; y++) {
        for (int32_t x = 0; x < 800; x++) {
            switch (board[y][x]) {
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
            board[y][x] = randColor;
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
        if (algorithm == alg_linear) {
            for (int32_t y = 600 - 1; y > 1; y--) {
                for (int32_t x = 800 - 1; x > 1; x--) {
                    int32_t similarFans = searchForSimilar(x, y, board[y][x]);
                    if (similarFans >= 4) { continue; }
                    else {
                        uint8_t newColor = sampleFanbase(x, y, board[y][x]);
                        board[y][x] = newColor;
                    }
                }
            }
        } else if (algorithm == alg_spiral) {
            //
        }

        outputPPM(iterations);
        iterations++;
    } while (iterations <= 50);

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
