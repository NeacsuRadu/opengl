#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ShaderProgram.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Camera.h>
#include <Callbacks.h>

#include <iostream>

unsigned int WINDOW_WIDTH = 800;
unsigned int WINDOW_HEIGHT = 600;

float g_currentFrame = 0.0f;

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

void init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int main()
{
    using namespace Engenius;
    init();
    GLFWwindow * pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rotation cube", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "Failed to create window." << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(pWindow);
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(pWindow, Callbacks::MouseMove);
    glfwSetScrollCallback(pWindow, Callbacks::MouseScroll);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD." << std::endl;
        glfwTerminate();
        return 0;
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(pWindow, Callbacks::FramebufferSize);

    std::string vertexShaderPath = "../shaders/vertex_shaders/lighting.vs";
    std::string fragmentShaderPath = "../shaders/fragment_shaders/lighting.fs";
    std::string lightSourceFSPath = "../shaders/fragment_shaders/lightSource.fs";
    CShaderProgram program(vertexShaderPath, fragmentShaderPath);
    program.Use();
    program.SetUniform("material.ambient", glm::vec3(1.0, 0.5, 0.31));
    program.SetUniform("material.diffuse", glm::vec3(1.0, 0.5, 0.31));
    program.SetUniform("material.specular", glm::vec3(0.5, 0.5, 0.5));
    program.SetUniform("material.shininess", 32.0f);
    program.SetUniform("light.ambient", glm::vec3(0.2, 0.2, 0.2));
    program.SetUniform("light.diffuse", glm::vec3(0.8, 0.8, 0.8));
    program.SetUniform("light.specular", glm::vec3(1.0, 1.0, 1.0));

    CShaderProgram lightSourceProgram(vertexShaderPath, lightSourceFSPath);
    CShaderProgram programContainer2(
        "../shaders/vertex_shaders/lighting_container2.vs",
        "../shaders/fragment_shaders/lighting_container2.fs"
    );

    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char *txt_data = stbi_load("../textures/container2.png", &width, &height, &channels, 0);
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

    txt_data = stbi_load("../textures/container2_specular.png", &width, &height, &channels, 0);
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

    txt_data = stbi_load("../textures/matrix.jpg", &width, &height, &channels, 0);
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
    programContainer2.SetUniform("light.constant", 1.0f);
    programContainer2.SetUniform("light.linear", 0.07f);
    programContainer2.SetUniform("light.quadratic", 0.017f);

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

    // GEN SHADOW MAP
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);

    while (!glfwWindowShouldClose(pWindow))
    {
        float currentTime = glfwGetTime();
        Camera::Instance().SetDeltaTime(currentTime - g_currentFrame);
        g_currentFrame = currentTime;

        process_input(pWindow);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float radius = 10;
        float x = std::sin(glfwGetTime()) * 3;
        float z = std::cos(glfwGetTime()) * 3;

        program.Use();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
        glm::mat4 view = Camera::Instance().GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(Camera::Instance().GetFov()), (float)WINDOW_WIDTH/ (float)WINDOW_HEIGHT, 0.1f, 100.0f);

        program.SetUniform("model", model);
        program.SetUniform("view", view);
        program.SetUniform("projection", projection);
        program.SetUniform("viewPos", Camera::Instance().GetPosition());
        program.SetUniform("light.position", glm::vec3(x, 1.0f, z));
        program.SetUniform("cameraDir", Camera::Instance().GetDirection());

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture1);  
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(cube_vertex_array);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        
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

        programContainer2.SetUniform("view", view);
        programContainer2.SetUniform("projection", projection);
        programContainer2.SetUniform("light.position", glm::vec3(x, 1.0f, z));
        programContainer2.SetUniform("viewPos", Camera::Instance().GetPosition());
        programContainer2.SetUniform("spotLight.direction", Camera::Instance().GetDirection());
        programContainer2.SetUniform("spotLight.position",  Camera::Instance().GetPosition());
        programContainer2.SetUniform("spotLight.innerCos", glm::cos(glm::radians(12.5f)));
        programContainer2.SetUniform("spotLight.outerCos", glm::cos(glm::radians(17.5f)));
        for (int i = 0; i < 10; ++i)
        {
            glm::mat4 model2 = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            programContainer2.SetUniform("model", model2);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}