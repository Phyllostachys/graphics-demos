#include <cstdint>
#include <ctime>
#include <iostream>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "lodepng.h"
#include "shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char** argv)
{
    // Setup GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#define FULLSCREEN 0
#if FULLSCREEN
    GLFWwindow* window = glfwCreateWindow(800, 600, "Texture Test", glfwGetPrimaryMonitor(), nullptr);
#else
    GLFWwindow* window = glfwCreateWindow(800, 600, "Texture Test", nullptr, nullptr);
#endif
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // Load in OGL functions
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // shader
    Shader s("assets/vert.glsl", "assets/frag.glsl");
    if(!s.compile()) {
        std::cout << "Some error happened while creating shaders\n";
        return -1;
    }

    float vertices[] = {
          /* position */     /* texcoords */
        -0.5, -0.5, 0.0,    0.0, 0.0,
        0.5, -0.5, 0.0,     1.0, 0.0,
        0.5, 0.5, 0.0,      1.0, 1.0,
        -0.5, 0.5, 0.0,     0.0, 1.0,
    };

    float tex_coords[] = {
        0.5, 0.2,
        1.0, 1.0,
        0.5, 0.8,
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

    glBindVertexArray(0); // Unbind VAO

    // load in texture data
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set our texture parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    uint32_t width, height;
    std::vector<uint8_t> image;

    //*
    uint32_t error = lodepng::decode(image, width, height, "asset.png");
    if(error) {
        std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
    //*/

    /* create random image data
    width = 64;
    height = 64;
    for (uint32_t y = 0; y < 4*64; y++) {
        for (uint32_t x = 0; x < 4*64; x++) {
            image.push_back(rand() % 255);
        }
    }
    //*/

    /* spit out image data
    std::cout << "asset.png data:\n";
    std::cout << "  width = " << width << std::endl;
    std::cout << "  height = " << height << std::endl;

    std::cout << std::hex << std::uppercase;
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x+=3) {
            std::cout << "0x" << (uint32_t)image[y*width + x] << (uint32_t)image[y*width + x+1] << (uint32_t)image[y*width + x+2] << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::dec;
    //*/

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    //glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(s.getShaderProgram(), "textureData"), 0);

        // Activate shader
        glUseProgram(s.getShaderProgram());

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
