#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ShaderProgram.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

unsigned int WINDOW_WIDTH = 800;
unsigned int WINDOW_HEIGHT = 600;

float g_cameraSpeed = 0.5f;
float g_deltaTime = 0.0f;
float g_currentFrame = 0.0f;

float g_lastX = 400.0f;
float g_lastY = 300.0f;
float g_pitch = 0.0f;
float g_yaw = -90.0f;
float g_fov = 45.0f;

bool g_firstMouse = true;

glm::vec3 g_cameraPosition(0.0f, 0.0f, 3.0f);
glm::vec3 g_cameraDirection(0.0f, 0.0f, -1.0f);
glm::vec3 g_worldUp(0.0f, 1.0f, 0.0f);


void framebuffer_size_callback(GLFWwindow * pWindow, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow * pWindow, double xpos, double ypos)
{
    if (g_firstMouse)
    {
        g_firstMouse = false;
        g_lastX = xpos;
        g_lastY = ypos;
    }

    float xoffset = xpos - g_lastX;
    float yoffset = g_lastY - ypos;
    g_lastX = xpos;
    g_lastY = ypos;

    float sensivity = 0.05f;
    xoffset *= sensivity;
    yoffset *= sensivity;

    g_yaw += xoffset;
    g_pitch += yoffset;

    if(g_pitch > 89.0f)
        g_pitch = 89.0f;
    if(g_pitch < -89.0f)
        g_pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(g_yaw));// * cos(glm::radians(g_pitch));
    front.y = sin(glm::radians(g_pitch));
    front.z = sin(glm::radians(g_yaw));// * cos(glm::radians(g_pitch));
    g_cameraDirection = glm::normalize(front);
}

void scroll_callback(GLFWwindow * pWindow, double xoffset, double yoffset)
{
    if (g_fov >= 1.0f && g_fov <= 45.0f)
        g_fov -= yoffset;
    if (g_fov <= 1.0f)
        g_fov = 1.0f;
    if (g_fov >= 45.0f)
        g_fov = 45.0f;
}

void process_input(GLFWwindow * pWindow)
{
    g_cameraSpeed = 2.5f * g_deltaTime;
    if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, true);
    
    glm::vec3 movingDirection = g_cameraDirection;
    movingDirection.y = 0.0f;
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
        g_cameraPosition += g_cameraSpeed * movingDirection;

    if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
        g_cameraPosition -= g_cameraSpeed * movingDirection;

    if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
        g_cameraPosition -= g_cameraSpeed * glm::normalize(glm::cross(movingDirection, g_worldUp));

    if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
        g_cameraPosition += g_cameraSpeed * glm::normalize(glm::cross(movingDirection, g_worldUp));
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rotation cube", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "Failed to create window." << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(pWindow);
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(pWindow, mouse_callback);
    glfwSetScrollCallback(pWindow, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD." << std::endl;
        glfwTerminate();
        return 0;
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);

    std::string vertexShaderPath = "shaders/vertex_shaders/rotating_cube.vs";
    std::string fragmentShaderPath = "shaders/fragment_shaders/rotating_cube.fs";
    CShaderProgram program(vertexShaderPath, fragmentShaderPath);

    int width, height, channels;
    unsigned char *txt_data = stbi_load("textures/container.jpg", &width, &height, &channels, 0);
    if (!txt_data)
        std::cout << "Failed to load texture" << std::endl;

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, txt_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(txt_data);

    stbi_set_flip_vertically_on_load(true);
    txt_data = stbi_load("textures/awesomeface.png", &width, &height, &channels, 0);
    if (!txt_data)
        std::cout << "Failed to load awesomeface.png texture" << std::endl;

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, txt_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(txt_data);

    program.Use();
    glUniform1i(glGetUniformLocation(program.GetId(), "texture1"), 0);
    glUniform1i(glGetUniformLocation(program.GetId(), "texture2"), 1);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int cube_vertex_buffer;
    glGenBuffers(1, &cube_vertex_buffer);

    unsigned int cube_vertex_array;
    glGenVertexArrays(1, &cube_vertex_array);
    glBindVertexArray(cube_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, cube_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    while (!glfwWindowShouldClose(pWindow))
    {
        float currentTime = glfwGetTime();
        g_deltaTime = currentTime - g_currentFrame;
        g_currentFrame = currentTime;

        process_input(pWindow);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.Use();

        glm::mat4 view = glm::lookAt(g_cameraPosition, g_cameraPosition + g_cameraDirection, g_worldUp);
        glm::mat4 projection = glm::perspective(glm::radians(g_fov), (float)WINDOW_WIDTH/ (float)WINDOW_HEIGHT, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(program.GetId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(program.GetId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(cube_vertex_array);
        for (int i = 0; i < 10; ++i)
        {
            // glm::radians(static_cast<float>(i) * 20.0f)
            glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f * (float)i), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(program.GetId(), "model"), 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}