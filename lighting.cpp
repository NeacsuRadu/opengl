#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ShaderProgram.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    //movingDirection.y = 0.0f;
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

    std::string vertexShaderPath = "shaders/vertex_shaders/lighting.vs";
    std::string fragmentShaderPath = "shaders/fragment_shaders/lighting.fs";
    std::string lightSourceFSPath = "shaders/fragment_shaders/lightSource.fs";
    CShaderProgram program(vertexShaderPath, fragmentShaderPath);
    program.Use();
    program.SetUniform("material.ambient", glm::vec3(1.0, 0.5, 0.31));
    program.SetUniform("material.diffuse", glm::vec3(1.0, 0.5, 0.31));
    program.SetUniform("material.specular", glm::vec3(0.5, 0.5, 0.5));
    program.SetUniform("material.shininess", 32.0f);
    program.SetUniform("light.ambient", glm::vec3(0.2, 0.2, 0.2));
    program.SetUniform("light.diffuse", glm::vec3(0.5, 0.5, 0.5));
    program.SetUniform("light.specular", glm::vec3(1.0, 1.0, 1.0));

    CShaderProgram lightSourceProgram(vertexShaderPath, lightSourceFSPath);
    CShaderProgram programContainer2(
        "shaders/vertex_shaders/lighting_container2.vs",
        "shaders/fragment_shaders/lighting_container2.fs"
    );

    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char *txt_data = stbi_load("textures/container2.png", &width, &height, &channels, 0);
    if (!txt_data)
        std::cout << "Failed to load texture" << std::endl;

    unsigned int txtContainer2;
    glGenTextures(1, &txtContainer2);
    glBindTexture(GL_TEXTURE_2D, txtContainer2);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, txt_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(txt_data);

    txt_data = stbi_load("textures/container2_specular.png", &width, &height, &channels, 0);
    if (!txt_data)
        std::cout << "Failed to load texture" << std::endl;

    unsigned int txtContainer2Sp;
    glGenTextures(1, &txtContainer2Sp);
    glBindTexture(GL_TEXTURE_2D, txtContainer2Sp);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, txt_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(txt_data);

    txt_data = stbi_load("textures/matrix.jpg", &width, &height, &channels, 0);
    if (!txt_data)
        std::cout << "Failed to load texture" << std::endl;

    unsigned int txtMatrix;
    glGenTextures(1, &txtMatrix);
    glBindTexture(GL_TEXTURE_2D, txtMatrix);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, txt_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(txt_data);


    programContainer2.Use();
    programContainer2.SetUniform("material.diffuse", 0);
    programContainer2.SetUniform("material.specular", 1);
    programContainer2.SetUniform("emission", 2);
    programContainer2.SetUniform("material.shininess", 32.0f);
    programContainer2.SetUniform("light.ambient", glm::vec3(0.2, 0.2, 0.2));
    programContainer2.SetUniform("light.diffuse", glm::vec3(0.5, 0.5, 0.5));
    programContainer2.SetUniform("light.specular", glm::vec3(1.0, 1.0, 1.0));

    // stbi_set_flip_vertically_on_load(true);
    // txt_data = stbi_load("textures/awesomeface.png", &width, &height, &channels, 0);
    // if (!txt_data)
    //     std::cout << "Failed to load awesomeface.png texture" << std::endl;

    // unsigned int texture2;
    // glGenTextures(1, &texture2);
    // glBindTexture(GL_TEXTURE_2D, texture2);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, txt_data);
    // glGenerateMipmap(GL_TEXTURE_2D);

    // stbi_image_free(txt_data);

    // program.Use();
    // glUniform1i(glGetUniformLocation(program.GetId(), "texture1"), 0);
    // glUniform1i(glGetUniformLocation(program.GetId(), "texture2"), 1);

    float vertices[] = {
       // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f 
    };

    unsigned int cube_vertex_buffer;
    glGenBuffers(1, &cube_vertex_buffer);

    unsigned int cube_vertex_array;
    glGenVertexArrays(1, &cube_vertex_array);
    glBindVertexArray(cube_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, cube_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    unsigned int light_source_vao;
    glGenVertexArrays(1, &light_source_vao);
    glBindVertexArray(light_source_vao);

    glBindBuffer(GL_ARRAY_BUFFER, cube_vertex_buffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int vao_container2;
    glGenVertexArrays(1, &vao_container2);
    glBindVertexArray(vao_container2);

    glBindBuffer(GL_ARRAY_BUFFER, cube_vertex_buffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    while (!glfwWindowShouldClose(pWindow))
    {
        float currentTime = glfwGetTime();
        g_deltaTime = currentTime - g_currentFrame;
        g_currentFrame = currentTime;

        process_input(pWindow);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float radius = 10;
        float x = std::sin(glfwGetTime()) * 3;
        float z = std::cos(glfwGetTime()) * 3;

        program.Use();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
        glm::mat4 view = glm::lookAt(g_cameraPosition, g_cameraPosition + g_cameraDirection, g_worldUp);
        glm::mat4 projection = glm::perspective(glm::radians(g_fov), (float)WINDOW_WIDTH/ (float)WINDOW_HEIGHT, 0.1f, 100.0f);

        program.SetUniform("model", model);
        program.SetUniform("view", view);
        program.SetUniform("projection", projection);
        program.SetUniform("viewPos", g_cameraPosition);
        program.SetUniform("light.position", glm::vec3(x, 1.0f, z));

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture1);  
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(cube_vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glBindVertexArray(light_source_vao);
        lightSourceProgram.Use();
        
        glm::mat4 lightSourceModel = glm::translate(glm::mat4(1.0f), glm::vec3(x, 1.0f, z));
        lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.2));
        lightSourceProgram.SetUniform("model", lightSourceModel);
        lightSourceProgram.SetUniform("view", view);
        lightSourceProgram.SetUniform("projection", projection);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        programContainer2.Use();
        glBindVertexArray(vao_container2);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, txtContainer2);

        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, txtContainer2Sp);

        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, txtMatrix);

        programContainer2.SetUniform("model", glm::mat4(1.0f));
        programContainer2.SetUniform("view", view);
        programContainer2.SetUniform("projection", projection);
        programContainer2.SetUniform("light.position", glm::vec3(x, 1.0f, z));
        programContainer2.SetUniform("viewPos", g_cameraPosition);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}