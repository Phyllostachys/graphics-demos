#include <iostream>
#include <fstream>

#include "GL/gl3w.h"
#include "shader_util.h"
#define DEBUG

GLuint compile_shaders(void)
{
    GLuint vertShader;
    GLuint fragShader;
    GLuint program;

    std::ifstream ifs;
    std::string inBuffer;

    // vertex shader
    ifs.open("vert_shader.vert", std::ifstream::in);
    char c = ifs.get();
    while (ifs.good()) {
        inBuffer.push_back(c);
        std::cout << c;
        c = ifs.get();
    }
    const GLchar* vertexShaderSource = inBuffer.c_str();
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertShader);

#ifdef DEBUG
    GLsizei length;
    GLchar logBuffer[1024];
    glGetShaderInfoLog(vertShader, 1024, &length, logBuffer);
    if (length > 0) { std::cout << logBuffer << std::endl; }
#endif
    inBuffer.clear();
    ifs.close();

    // fragment shader
    ifs.open("frag_shader.frag", std::ifstream::in);
    c = ifs.get();
    while (ifs.good()) {
        inBuffer.push_back(c);
        std::cout << c;
        c = ifs.get();
    }
    const GLchar* fragmentShaderSource = inBuffer.c_str();
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragShader);

#ifdef DEBUG
    glGetShaderInfoLog(vertShader, 1024, &length, logBuffer);
    if (length > 0) { std::cout << logBuffer << std::endl; }
#endif
    inBuffer.clear();
    ifs.close();

    // create program
    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    // cleanup
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}
