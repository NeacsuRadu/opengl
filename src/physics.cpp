#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Callbacks.h>
#include <Camera.h>
#include <ShaderProgram.h>
#include <Object.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "react/reactphysics3d.h"

#include <iostream>

unsigned int WINDOW_WIDTH  = 800;
unsigned int WINDOW_HEIGHT = 600;

float g_currentFrametime = 0;
float g_deltaTime = 0;

void process_input(GLFWwindow * pWindow)
{
    using namespace Engenius;
    if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, true);
    else if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
        Camera::Instance().UpdatePositionFront();
    else if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
        Camera::Instance().UpdatePositionBack();
    else if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
        Camera::Instance().UpdatePositionLeft();
    else if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
        Camera::Instance().UpdatePositionRight();
}

float get_lat(int input, int ibegin = 0, int iend = 20, float obegin = glm::radians(0.0f), float oend = glm::radians(180.0f))
{
    return (obegin + input * ((oend - obegin) / iend));
}

float get_long(int input, int ibegin = 0, int iend = 20, float obegin = glm::radians(0.0f), float oend = glm::radians(360.0f))
{
    return (obegin + input * ((oend - obegin) / iend));
}

void add_point(int& count, float * vec, float latitudine, float longitudine)
{
    vec[count ++] = 2.0 * glm::sin(longitudine) * glm::cos(latitudine);
    vec[count ++] = 2.0 * glm::sin(longitudine) * glm::sin(latitudine);
    vec[count ++] = 2.0 * glm::cos(longitudine);
}

void add_point(int& count, float * vec, float x, float y, float z)
{
    vec[count ++] = 0.1 * x - 0.5;
    vec[count ++] = 0.1 * y - 0.5;
    vec[count ++] = z;
}

int main()
{
    using namespace Engenius;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Widow", nullptr, nullptr);
    if (!pWindow)
    {   
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(pWindow);
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(pWindow, Engenius::Callbacks::MouseMove);
    glfwSetScrollCallback(pWindow, Engenius::Callbacks::MouseScroll);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD." << std::endl;
        glfwTerminate();
        return 0;
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(pWindow, Engenius::Callbacks::FramebufferSize);

    CShaderProgram program(
        "../shaders/vertex_shaders/gravity.vs",
        "../shaders/fragment_shaders/gravity.fs"
    );

    float long_begin = glm::radians(-180.0f);
    float long_end   = glm::radians( 180.0f);

    float lat_begin = glm::radians( 90.0f);
    float lat_end   = glm::radians(-90.0f);

    float vertices[ 20 * 20 * 2 * 3 * 3];
    int count = 0;
    unsigned int granulation = 20;
    float raza = 1.0;

    // for (int j = 0; j <= granulation; ++j)
    // {
    //     float lat = get_lat(granulation);
    //     float lon = get_long(j);

    //     float x = 2.0f * glm::sin(lat) * glm::cos(lon);
    //     float y = 2.0f * glm::sin(lat) * glm::sin(lon);
    //     float z = 2.0f * glm::cos(lat);

    //     std::cout << x << " " << y << " " << z << std::endl;

    //     vertices[count ++] = x;
    //     vertices[count ++] = y;
    //     vertices[count ++] = z;
    // }

    for (int i = 0; i < granulation; ++ i)
    {
        /*
        [0, x], [0, y]  -->  pt orice t din primul t' = (y/ x) * t


        input_interval = [0, 199] - 200 de elem -- asta e deja cum trebuie
        output_interval = [-pi/2, pi/2] tot 200 de elem -- t --> -pi/2 + (pi/2 - (-pi/2)) / 
        */
        for (int j = 1; j <= granulation; ++ j)
        {
            add_point(count, vertices, get_lat(i), get_long(j));
            add_point(count, vertices, get_lat(i), get_long(j - 1));
            add_point(count, vertices, get_lat(i + 1), get_long(j - 1));

            add_point(count, vertices, get_lat(i), get_long(j));
            add_point(count, vertices, get_lat(i + 1), get_long(j - 1));
            add_point(count, vertices, get_lat(i + 1), get_long(j));

            // add_point(count, vertices, j, i, 0);
            // add_point(count, vertices, j - 1, i, 0);
            // add_point(count, vertices, j - 1, i + 1, 0);

            // add_point(count, vertices, j, i, 0);
            // add_point(count, vertices, j - 1, i + 1, 0);
            // add_point(count, vertices, j, i + 1, 0);
        }
    }

    // for (int i = 0; i < granulation - 1; ++ i)
    // {
    //     add_point(count, vertices, get_lat(i), get_long(granulation - 1));
    //     add_point(count, vertices, get_lat(i), get_long(0));
    //     add_point(count, vertices, get_lat(i + 1), get_long(0));

    //     add_point(count, vertices, get_lat(i), get_long(granulation - 1));
    //     add_point(count, vertices, get_lat(i + 1), get_long(0));
    //     add_point(count, vertices, get_lat(i + 1), get_long(granulation - 1));
    // }

    // for (int i = 0; i < 9; i ++)
    // {
    //     for (int j = 1; j < 10; j++)
    //     {
    //         vertices[count++] = 0.1 * j - 0.5;
    //         vertices[count++] = 0.1 * i - 0.5;
    //         vertices[count++] = 0;
    //         vertices[count++] = 0.1 * (j - 1) - 0.5;
    //         vertices[count++] = 0.1 * i - 0.5;
    //         vertices[count++] = 0;
    //         vertices[count++] = 0.1 * (j -1) - 0.5;
    //         vertices[count++] = 0.1 * (i + 1) - 0.5;
    //         vertices[count++] = 0;

    //         vertices[count++] = 0.1 * j - 0.5;
    //         vertices[count++] = 0.1 * i - 0.5;
    //         vertices[count++] = 0;
    //         vertices[count++] = 0.1 * (j -1) - 0.5;
    //         vertices[count++] = 0.1 * (i + 1) - 0.5;
    //         vertices[count++] = 0;
    //         vertices[count++] = 0.1 * j - 0.5;
    //         vertices[count++] = 0.1 * (i + 1) - 0.5;
    //         vertices[count++] = 0;
    //     }
    // }
    

    // float vertices[] = 
    // {
    //    // positions   
    //     -0.5f, -0.5f, -0.5f,
    //     0.5f, -0.5f, -0.5f, 
    //     0.5f,  0.5f, -0.5f, 
    //     0.5f,  0.5f, -0.5f, 
    //     -0.5f,  0.5f, -0.5f,
    //     -0.5f, -0.5f, -0.5f,

    //     -0.5f, -0.5f,  0.5f,
    //     0.5f, -0.5f,  0.5f, 
    //     0.5f,  0.5f,  0.5f, 
    //     0.5f,  0.5f,  0.5f, 
    //     -0.5f,  0.5f,  0.5f,
    //     -0.5f, -0.5f,  0.5f,

    //     -0.5f,  0.5f,  0.5f,
    //     -0.5f,  0.5f, -0.5f,
    //     -0.5f, -0.5f, -0.5f,
    //     -0.5f, -0.5f, -0.5f,
    //     -0.5f, -0.5f,  0.5f,
    //     -0.5f,  0.5f,  0.5f,

    //     0.5f,  0.5f,  0.5f, 
    //     0.5f,  0.5f, -0.5f, 
    //     0.5f, -0.5f, -0.5f, 
    //     0.5f, -0.5f, -0.5f, 
    //     0.5f, -0.5f,  0.5f, 
    //     0.5f,  0.5f,  0.5f, 

    //     -0.5f, -0.5f, -0.5f,
    //     0.5f, -0.5f, -0.5f, 
    //     0.5f, -0.5f,  0.5f, 
    //     0.5f, -0.5f,  0.5f, 
    //     -0.5f, -0.5f,  0.5f,
    //     -0.5f, -0.5f, -0.5f,

    //     -0.5f,  0.5f, -0.5f,
    //     0.5f,  0.5f, -0.5f, 
    //     0.5f,  0.5f,  0.5f, 
    //     0.5f,  0.5f,  0.5f, 
    //     -0.5f,  0.5f,  0.5f,
    //     -0.5f,  0.5f, -0.5f
    // };



    unsigned int vboCube;
    glGenBuffers(1, &vboCube);

    unsigned int vaoCube;
    glGenVertexArrays(1, &vaoCube);
    glBindVertexArray(vaoCube);

    glBindBuffer(GL_ARRAY_BUFFER, vboCube);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    GLenum e;
    float floorVertices[] = 
    {
        -50.0f, 0.0f, 50.0f,
        -50.0f, 0.0f, 50.0f,
        50.0f, 0.0f, 50.0f,
        50.0f, 0.0f, 50.0f,
        50.0f, 0.0f, -50.0f, 
        -50.0f, 0.0f, -50.0f
    };

    unsigned int vboFloor;
    glGenBuffers(1, &vboFloor);

    unsigned int vaoFloor;
    glGenVertexArrays(1, &vaoFloor);
    glBindVertexArray(vaoFloor);

    glBindBuffer(GL_ARRAY_BUFFER, vboFloor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    Object obj;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(pWindow))
    {
        float currentTime = glfwGetTime();
        g_deltaTime = currentTime - g_currentFrametime;
        g_currentFrametime = currentTime;
        Camera::Instance().SetDeltaTime(g_deltaTime);

        process_input(pWindow);

        glClearColor(0.61f, 0.74f, 0.94f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = obj.GetModelMatrix();
        glm::mat4 view = Camera::Instance().GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(Camera::Instance().GetFov()), (float)WINDOW_WIDTH/ (float)WINDOW_HEIGHT, 0.1f, 100.0f);

        program.Use();
        program.SetUniform("model", model);
        program.SetUniform("view", view);
        program.SetUniform("projection", projection);

        glBindVertexArray(vaoCube);
        glDrawArrays(GL_TRIANGLES, 0, int(count/3));

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}