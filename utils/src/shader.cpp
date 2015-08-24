#include <cstdio>
#include <iostream>
#include <glad/glad.h>

#include "shader.h"

// src - a file that has been opened already
// dest - a std::string where the contents of src will be put into
static void readFile(std::string dest, FILE* src)
{
    char* fileBuf;
    size_t fileSize;

    // get size of contents
    fseek(src, 0, SEEK_END);
    fileSize = ftell(src);

    // use size of contents
    dest.reserve(fileSize);
    fileBuf = new char[fileSize];

    // get contents
    rewind(src);
    fread(fileBuf, sizeof(char), fileSize, src);

    // assign contents and cleanup
    dest.assign(fileBuf);
    delete[] fileBuf;
}

Shader::Shader()
{
    anyErrors = false;
    typeFlags = 0;
    compiled = false;
    vertShaderHandle = 0;
    geomShaderHandle = 0;
    tessShaderHandle = 0;
    fragShaderHandle = 0;
}

Shader::Shader(std::string vert_filename, std::string frag_filename)
{
    FILE* inFile;

    inFile = fopen(vert_filename.c_str(), "r");
    if (inFile == NULL) {
        std::cout << "Vertex shader file doesn't exist.\n";
        anyErrors = true;
        return;
    }
    readFile(vertSource, inFile);
    fclose(inFile);

    inFile = fopen(frag_filename.c_str(), "r");
    if (inFile == NULL) {
        std::cout << "Fragment shader file doesn't exist.\n";
        anyErrors = true;
        return;
    }
    readFile(fragSource, inFile);
    fclose(inFile);

    typeFlags = VERT_SHADER | FRAG_SHADER;
    compiled = false;
    vertShaderHandle = 0;
    geomShaderHandle = 0;
    tessShaderHandle = 0;
    fragShaderHandle = 0;
}

int32_t Shader::addShader(std::string src_filename, shader_type type)
{
    return -1;
}

GLuint Shader::getShaderProgram()
{
    if (compiled == false) {
        std::cout << "Shader hasn't been compiled yet\n";
        anyErrors = true;
        return 0;
    }

    return shaderProgHandle;
}

static bool isShaderStatusGood(GLint shaderHandle, GLenum statusType, char errorMsg[1024])
{
    GLint status;
    glGetShaderiv(shaderHandle, statusType, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(shaderHandle, 1024, NULL, errorMsg);
        return false;
    } else {
        return true;
    }
}

static bool isShaderProgramStatusGood(GLint programHandle, GLenum statusType, char errorMsg[1024])
{
    GLint status;
    glGetProgramiv(programHandle, statusType, &status);
    if (status != GL_TRUE) {
        glGetProgramInfoLog(shaderHandle, 1024, NULL, errorMsg);
        return false;
    } else {
        return true;
    }
}

bool Shader::compile()
{
    char buffer[1024];

    // Vertex shader
    vertShaderHandle = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vs = vertSource.c_str();
    glShaderSource(vertShaderHandle, 1, &vs, NULL);
    glCompileShader(vertShaderHandle);

    // check vertex shader compile status
    if (!isShaderStatusGood(vertShaderHandle, GL_COMPILE_STATUS, buffer)) {
        anyErrors = true;
        std::cout << buffer << std::endl;
        return false;
    }

    // geometry shader
    // tesselation shader

    // fragment shader
    fragShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fs = fragSource.c_str();
    glShaderSource(fragShaderHandle, 1, &fs, NULL);
    glCompileShader(fragShaderHandle);

    // check vertex shader compile status
    if (!isShaderStatusGood(fragShaderHandle, GL_COMPILE_STATUS, buffer)) {
        anyErrors = true;
        std::cout << buffer << std::endl;
        return false;
    }

    // create and link full pipeline
    shaderProgHandle = glCreateProgram();
    glAttachShader(shaderProgHandle, vertShaderHandle);
    if (typeFlags & GEOM_SHADER) {
        glAttachShader(shaderProgHandle, geomShaderHandle);
    }
    if (typeFlags & TESS_SHADER) {
        glAttachShader(shaderProgHandle, tessShaderHandle);
    }
    glAttachShader(shaderProgHandle, fragShaderHandle);
    glLinkProgram(shaderProgHandle);
    // remember that a call to glUseProgram will still need to be done

    // check program link status
    if (!isProgramStatusGood(shaderProgHandle, GL_LINK_STATUS, buffer)) {
        anyErrors = true;
        std::cout << buffer << std::endl;
        return false;
    }

    return true;
}