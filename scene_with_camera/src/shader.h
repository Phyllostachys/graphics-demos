#ifndef SHADER_H_
#define SHADER_H_

#include <iostream>
#include <glad/glad.h>

enum shader_type {
    VERT_SHADER = 1,
    GEOM_SHADER = 2,
    TESS_SHADER = 4,
    FRAG_SHADER = 8
};

class Shader
{
public:
    bool anyErrors;

    Shader();
    Shader::Shader(std::string vert_filename, std::string geom_filename, std::string frag_filename);
    int32_t addShader(std::string src_filename, shader_type type);
    GLuint getShaderProgram();
    bool compile();

private:
    std::string vertSource;
    std::string geomSource;
    std::string fragSource;

    GLuint vertShaderHandle;
    GLuint geomShaderHandle;
    GLuint fragShaderHandle;
    GLuint shaderProgHandle;

    uint8_t typeFlags;
    bool compiled;
};

#endif // SHADER_H_
