#include <iostream>
#include <fstream>

#include "GL/gl3w.h"
#include "shader_util.h"
#define DEBUG
#define NO_FILE_SHADERS

static const GLchar vs[] = {
    "#version 430 core\n"
    "layout(location = 0) in vec4 position;"
    "void main(void)"
    "{"
    "    gl_Position = position;"
    "}"
};

static const GLchar fs[] = {
    "#version 430 core\n"
    "out vec4 color;"
    "void main(void)"
    "{"
    "    color = vec4(1.0, 0.0, 0.0, 1.0);"
    "}"
};

GLuint compile_shaders(void)
{
    GLuint vertShader;
    GLuint fragShader;
    GLuint program;

#ifdef NO_FILE_SHADERS
    const GLchar* vertexShaderSource = vs;
#else
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
#endif
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertShader);

#ifdef DEBUG
    GLsizei length;
    GLchar logBuffer[1024];
    glGetShaderInfoLog(vertShader, 1024, &length, logBuffer);
    if (length > 0) { std::cout << logBuffer << std::endl; }
#endif
#ifndef NO_FILE_SHADERS
    inBuffer.clear();
    ifs.close();
#endif

    // fragment shader
#ifdef NO_FILE_SHADERS
    const GLchar* fragmentShaderSource = fs;
#else
    ifs.open("frag_shader.frag", std::ifstream::in);
    c = ifs.get();
    while (ifs.good()) {
        inBuffer.push_back(c);
        std::cout << c;
        c = ifs.get();
    }
    const GLchar* fragmentShaderSource = inBuffer.c_str();
#endif
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragShader);

#ifdef DEBUG
    glGetShaderInfoLog(vertShader, 1024, &length, logBuffer);
    if (length > 0) { std::cout << logBuffer << std::endl; }
#endif
#ifndef NO_FILE_SHADERS
    inBuffer.clear();
    ifs.close();
#endif

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
