#include <cstdint>
#include <ctime>
#include <iostream>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define COLOR_INC_VALUE 2;

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
    uniform vec3 spherePos1;
    uniform float sphereRadius1;
    uniform vec3 spherePos2;
    uniform float sphereRadius2;
    uniform vec3 spherePos3;
    uniform float sphereRadius3;

    // Output
    out vec4 outColor;

    // Globals
    vec3 light = vec3(800.0, 450.0, 100.0);

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
        //vec3 spherePos = vec3(800.0, 450.0, -20.0);
        //float sphereRadius = 175.0;
        light.x = light.x + 200 * sin(mod(time, 60.0) * 3.14);
        light.y = light.y + 200 * cos(mod(time, 60.0) * 3.14);
        //light.z = light.z + 10 * sin(mod(time, 60.0) * 3.14);

        if (length(gl_FragCoord.xyz - spherePos1) > sphereRadius1 &&
            length(gl_FragCoord.xyz - spherePos2) > sphereRadius2 &&
            length(gl_FragCoord.xyz - spherePos3) > sphereRadius3) {
            discard;
        }

        vec3 rayOrigin = gl_FragCoord.xyz;
        rayOrigin.z = 0.0;
        vec3 rayDir = normalize(vec3(0.0, 0.0, -1.0));

        // Intersect ray with spheres
        float t1 = intersect(rayOrigin, rayDir, spherePos1, sphereRadius1);
        float t2 = intersect(rayOrigin, rayDir, spherePos2, sphereRadius2);
        float t3 = intersect(rayOrigin, rayDir, spherePos3, sphereRadius3);

        float t;
        vec3 spherePos;
        if (t1 > 0.0 && t1 < t2 && t1 < t3) {
            t = t1;
            spherePos = spherePos1;
        } else if (t2 > 0.0 && t2 < t1 && t2 < t3) {
            t = t2;
            spherePos = spherePos2;
        } else if (t3 > 0.0 && t3 < t1 && t3 < t2) {
            t = t3;
            spherePos = spherePos3;
        } else {
            outColor = 0.5 * vec4(triangleColor, 1.0);
            return;
        }

        vec3 intersectionPoint = rayOrigin + t * rayDir;
        vec3 sphereNormal = normalize(intersectionPoint - spherePos);
        float intensity = dot(sphereNormal, intersectionPoint - light) * .2;

        outColor = vec4((intensity / 100.0) * triangleColor, 1.0);
    }
    )***";

int main()
{
    // GLFW3 init stuff
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "Ray Tracing Screensaver", glfwGetPrimaryMonitor(), nullptr);
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::default_random_engine rand(time(NULL));

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
    GLint spherePos1 = glGetUniformLocation(shaderProgram, "spherePos1");
    GLint sphereRadius1 = glGetUniformLocation(shaderProgram, "sphereRadius1");
    GLint spherePos2 = glGetUniformLocation(shaderProgram, "spherePos2");
    GLint sphereRadius2 = glGetUniformLocation(shaderProgram, "sphereRadius2");
    GLint spherePos3 = glGetUniformLocation(shaderProgram, "spherePos3");
    GLint sphereRadius3 = glGetUniformLocation(shaderProgram, "sphereRadius3");

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

    float spherePositions[3][3] = {
        { (float)(rand() % 1300) + 149.0f, (float)(rand() % 600) + 149.0f, -20.0f },
        { (float)(rand() % 1300) + 149.0f, (float)(rand() % 600) + 149.0f, -20.0f },
        { (float)(rand() % 1300) + 149.0f, (float)(rand() % 600) + 149.0f, -20.0f }
    };

    float sphereRadii[3] = {
        (float)(rand() % 125) + 10.0f, (float)(rand() % 125) + 10.0f, (float)(rand() % 125) + 10.0f
    };

    uint32_t counter = 0;
    while (!glfwWindowShouldClose(window)) {
        // handle events
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) || glfwGetKey(window, GLFW_KEY_ENTER) || glfwGetKey(window, GLFW_KEY_SPACE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (glfwGetTime() - startTime > 0.032) {
            std::cout << "Frame time: " << glfwGetTime() - startTime << std::endl;
            startTime = glfwGetTime();
            std::cout << (float)(rand() % 125) << std::endl;

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

            if (counter >= 250) {
                spherePositions[0][0] = (float)(rand() % 1300) + 149.0f;
                spherePositions[1][0] = (float)(rand() % 1300) + 149.0f;
                spherePositions[2][0] = (float)(rand() % 1300) + 149.0f;
                spherePositions[0][1] = (float)(rand() % 600) + 149.0f;
                spherePositions[1][1] = (float)(rand() % 600) + 149.0f;
                spherePositions[2][1] = (float)(rand() % 600) + 149.0f;
                spherePositions[0][2] = -20.0f;
                spherePositions[1][2] = -20.0f;
                spherePositions[2][2] = -20.0f;

                sphereRadii[0] = (float)(rand() % 125) + 10.0f;
                sphereRadii[1] = (float)(rand() % 125) + 10.0f;
                sphereRadii[2] = (float)(rand() % 125) + 10.0f;

                counter = 0;
            }

            // rendering
            //// make vbo the active buffer
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            //// copy vertices to active buffer
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glUseProgram(shaderProgram);

            //// actual drawing
            glClear(GL_COLOR_BUFFER_BIT);

            // Set Uniforms
            glUniform3f(uniColor, (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
            glUniform1f(time, (float)glfwGetTime());
            glUniform3f(spherePos1, spherePositions[0][0], spherePositions[0][1], spherePositions[0][2]);
            glUniform1f(sphereRadius1, sphereRadii[0]);
            glUniform3f(spherePos2, spherePositions[1][0], spherePositions[1][1], spherePositions[1][2]);
            glUniform1f(sphereRadius2, sphereRadii[1]);
            glUniform3f(spherePos3, spherePositions[2][0], spherePositions[2][1], spherePositions[2][2]);
            glUniform1f(sphereRadius3, sphereRadii[2]);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

            // swapping buffers and polling input
            glfwSwapBuffers(window);

            counter++;
        }
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
