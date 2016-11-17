#include <cstdint>
#include <ctime>
#include <iostream>
#include <random>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include "lodepng.h"
#include "Shader.h"

#define GLSL(x) #x

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char** argv)
{
    // Setup GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    int width, height;
#define FULLSCREEN 0
#if FULLSCREEN
    width = 1920;
    height = 1080;
#else
    width = 800;
    height = 600;
#endif

    GLFWwindow* window = glfwCreateWindow(width, height, "Texture Test", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // Load in OGL functions
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // shader
    Shader s("simple.vert", "hexes.frag");
    if(!s.compile()) {
        std::cout << "Some error happened while creating shaders\n";
        return -1;
    }

    float vertices[] = {
          /* position */     /* texcoords */
        -1.0, -1.0, 0.0,    0.0, 0.0,
        1.0, -1.0, 0.0,     1.0, 0.0,
        1.0, 1.0, 0.0,      1.0, 1.0,
        -1.0, 1.0, 0.0,     0.0, 1.0,
    };

    GLuint vertex_indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // load in vertex data
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // TexCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

    GLint textureDataSamplerLoc = glGetUniformLocation(s.getShaderProgram(), "textureData");
    GLint timeUniformLoc = glGetUniformLocation(s.getShaderProgram(), "time");
    GLint iResolutionUniformLoc = glGetUniformLocation(s.getShaderProgram(), "iResolution");

    // main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate shader
        glUseProgram(s.getShaderProgram());

        // set uniforms
        glUniform1i(textureDataSamplerLoc, 0);
        glUniform1f(timeUniformLoc, (float)glfwGetTime() + 5.0f);
        glUniform2f(iResolutionUniformLoc, (float)width, (float)height);

        // Draw container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}