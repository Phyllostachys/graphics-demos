#include <iostream>
#include <string>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define COLOR_INC_VALUE 3;

std::string vert_shader =
    R"***(
    #version 440
    in vec2 position;
    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
    )***";

std::string frag_shader =
    R"***(
    #version 440

    // Uniforms
    uniform vec3 triangleColor;
    uniform float time;
    out vec4 outColor;
    vec3 light = vec3(800.0, 450.0, 200.0);

    float intersect(vec3 ray, vec3 dir, vec3 center, float radius)
    {
	    vec3 oc = center - ray;
	    float dot_loc = dot(dir, oc);
	    float lsq = dot(dir, dir);
	    float discriminant = (dot_loc * dot_loc) - lsq * (dot(oc, oc) - (radius * radius));

	    if (discriminant < 0.0) {
		    return -1.0;
	    }

	    float i0 = (-dot_loc - sqrt(discriminant)) / lsq;
	    if (i0 >= 0.0) {
		    return i0;
	    }
	    float i1 = (-dot_loc + sqrt(discriminant)) / lsq;
	    return i1;
    }

    void main()
    {
        outColor = vec4(triangleColor, 1.0);
        vec3 spherePos = vec3(800.0, 450.0, -20.0);
        float sphereRadius = 175.0;
        light.x = light.x + 100 * sin(mod(time, 60.0) * 3.14);
        light.y = light.y + 100 * cos(mod(time, 60.0) * 3.14);
        //light.z = light.z + 10 * sin(mod(time, 60.0) * 3.14) + 10 * cos(mod(time, 60.0) * 3.14);

        if (length(gl_FragCoord.xyz - spherePos) > sphereRadius)
            discard;

        vec3 rayOrigin = gl_FragCoord.xyz;
        rayOrigin.z = 0.0;
        vec3 rayDir = normalize(vec3(0.0, 0.0, -1.0));

        float t = intersect(rayOrigin, rayDir, spherePos, sphereRadius);
        if (t < 0.0) {
            outColor = 0.5 * vec4(triangleColor, 1.0);
            return;
        }

        vec3 intersectionPoint = rayOrigin + t * rayDir;
        vec3 sphereNormal = normalize(intersectionPoint - spherePos);
        rayDir = normalize(rayOrigin - intersectionPoint);
        rayDir = reflect(rayDir, sphereNormal);
        float intensity = dot(sphereNormal, intersectionPoint - light) * .2;

        outColor = vec4((intensity / 100.0) * triangleColor, 1.0);
    }
    )***";

extern "C" void error_callback(int error, const char* description)
{
    std::cout << "ERROR " << error << " -- " <<  description << std::endl;;
}

int main()
{
    // GLFW3 init stuff
    glfwInit();
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "Ray Tracing Screensaver", glfwGetPrimaryMonitor(), nullptr);
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLAD init stuff
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    float vertices[] = {
        -1.0f, 1.0f, // Top Left Corner
        1.0f, 1.0f,  // Top Right Corner
        1.0f, -1.0f, // Bottom Right Corner
        -1.0f, -1.0f // Bottom Left Corner
    };

    // generate vertex buffer object
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // make vbo the active buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // copy vertices to active buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vs = vert_shader.c_str();
    glShaderSource(vertexShader, 1, &vs, NULL);
    glCompileShader(vertexShader);

    // check vertex shader compile status
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
        std::cout << buffer << std::endl;
        glfwTerminate();
        return 0;
    }

    // fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fs = frag_shader.c_str();
    glShaderSource(fragmentShader, 1, &fs, NULL);
    glCompileShader(fragmentShader);

    // check fragment shader compile status
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
        std::cout << buffer << std::endl;
        glfwTerminate();
        return 0;
    }

    // create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
    GLint time = glGetUniformLocation(shaderProgram, "time");

    // setup vertex array object (attribute object)
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);

    unsigned int r = 50, g = 50, b = 50;
    double startTime = glfwGetTime();
    enum {
        redUp,
        redDown,
        greenUp,
        greenDown,
        blueUp,
        blueDown
    } color_inc_state = redUp;

    while (!glfwWindowShouldClose(window)) {
        // handle events
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) || glfwGetKey(window, GLFW_KEY_ENTER) || glfwGetKey(window, GLFW_KEY_SPACE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (glfwGetTime() - startTime > 0.032) {
            startTime = glfwGetTime();

            switch (color_inc_state) {
                case redUp:
                    if (r >= 255) {
                        r = 255;
                        color_inc_state = redDown;
                    } else {
                        r += COLOR_INC_VALUE;
                    }
                    break;

                case redDown:
                    if (r <= 50) {
                        r = 50;
                        color_inc_state = greenUp;
                    } else {
                        r -= COLOR_INC_VALUE;
                    }
                    break;

                case greenUp:
                    if (g >= 255) {
                        g = 255;
                        color_inc_state = greenDown;
                    } else {
                        g += COLOR_INC_VALUE;
                    }
                    break;

                case greenDown:
                    if (g <= 50) {
                        g = 50;
                        color_inc_state = blueUp;
                    } else {
                        g -= COLOR_INC_VALUE;
                    }
                    break;

                case blueUp:
                    if (b >= 255) {
                        b = 255;
                        color_inc_state = blueDown;
                    } else {
                        b += COLOR_INC_VALUE;
                    }
                    break;

                case blueDown:
                    if (b <= 50) {
                        b = 50;
                        color_inc_state = redUp;
                    } else {
                        b -= COLOR_INC_VALUE;
                    }
                    break;

                default:
                    break;
            }

            // rendering
            //// make vbo the active buffer
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            //// copy vertices to active buffer
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glUseProgram(shaderProgram);

            //// actual drawing
            glClear(GL_COLOR_BUFFER_BIT);

            glUniform3f(uniColor, (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
            glUniform1f(time, (float)glfwGetTime());
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

            // swapping buffers and polling input
            glfwSwapBuffers(window);
        }
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
