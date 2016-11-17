#include "cube.h"
#include "shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <cstdio>
#include <iostream>

glm::vec3 cameraPos   = glm::vec3(0.0, 0.05, 23.0);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    GLfloat cameraSpeed = 0.05f;
    bool showNewPosition = false;
    switch (key) {
        case GLFW_KEY_W: {
            cameraPos += cameraSpeed * cameraFront;
            showNewPosition = true;
            break;
        }

        case GLFW_KEY_S: {
            cameraPos -= cameraSpeed * cameraFront;
            showNewPosition = true;
            break;
        }

        case GLFW_KEY_A: {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            showNewPosition = true;
            break;
        }

        case GLFW_KEY_D: {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            showNewPosition = true;
            break;
        }

        case GLFW_KEY_Q: {
            cameraPos += cameraSpeed * cameraUp;
            showNewPosition = true;
            break;
        }

        case GLFW_KEY_Z: {
            cameraPos -= cameraSpeed * cameraUp;
            showNewPosition = true;
            break;
        }

        default: {
            break;
        }
    }

    if (showNewPosition) {
        printf("%02F,%02F,%02F\n", cameraPos.x, cameraPos.y, cameraPos.z);
    }
}

int main(int argc, char** argv)
{
    int result;
    int width, height;

    result = glfwInit();
    if (result == GLFW_FALSE) {
        printf("Problem initializing GLFW3.");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Scene with Camera", nullptr, nullptr);
    if (window == nullptr) {
        printf("Problem creating window context.");
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Load in OGL functions
    if (!gladLoadGL()) {
        printf("Failed to initialize OpenGL context");
        return -1;
    }

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLuint VAO;
    GLuint vertBufferIndex[2];
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, vertBufferIndex);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vertBufferIndex[0]);
    glBufferData(GL_ARRAY_BUFFER, NUM_CUBE_VERTS * sizeof(float), cubeVerts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertBufferIndex[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_CUBE_IND * sizeof(unsigned int), cubeIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    // shaders
    Shader s("assets\\basic.vert", "assets\\basic.geom", "assets\\basic.frag");
    if(!s.compile()) {
        printf("Some error happened while creating shaders\n");
        return -1;
    }
    GLint mvpLoc = glGetUniformLocation(s.getShaderProgram(), "MVP");
    GLint timeLoc = glGetUniformLocation(s.getShaderProgram(), "time");

/*
    glm::mat4 model;
    model = glm::scale(model, glm::vec3(0.6, 0.6, 0.6));
    model = glm::rotate(model, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0, 0.0, 2.0));
    //*
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%2.3f\t", model[j][i]);
            if (j == 3)
                printf("\n");
        }
    }

    printf("\n");
    ///

    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0, 0.0, 7.0),
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0));
    /*
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%2.3f\t", view[j][i]);
            if (j == 3)
                printf("\n");
        }
    }
    /

    glm::mat4 proj = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);//glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f);
    /*
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%2.3f\t", view[j][i]);
            if (j == 3)
                printf("\n");
        }
    }

    printf("\n");
    /

    glm::mat4 mvp = proj * view * model;//proj * view * model;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%2.3f\t", mvp[j][i]);
            if (j == 3)
                printf("\n");
        }
    }
    //*/

    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        glm::mat4 center, scale, rotate, translate;
        center = glm::translate(center, glm::vec3(-0.5, -0.5, -0.5));
        //scale = glm::scale(scale, glm::vec3(1.6, 1.6, 1.6));
        rotate = glm::rotate(rotate, (GLfloat)glfwGetTime() * 1.0f, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
        translate = glm::translate(translate, glm::vec3(0.0, 0.0, 20.0));

        glm::mat4 model;
        model = translate * rotate * scale * center * model;
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 proj = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
        glm::mat4 mvp = proj * view * model;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(s.getShaderProgram());
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
        glUniform1f(timeLoc, (GLfloat)glfwGetTime() * 1.5);
        glUniform3f(glGetUniformLocation(s.getShaderProgram(), "drawColor"), 0.0f, 1.0f, 0.0f);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, NUM_CUBE_IND, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
