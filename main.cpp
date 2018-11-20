#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ShaderProgram.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow * pWindow, int width, int height)
{
    glViewport(0, 0, width, height);
}

float fs_alpha = 0.2;
float model_rotation_along_x_axis = -45.0f;
float view_translation_along_z_axis = -3.0f;

void process_input(GLFWwindow * pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, true);

    if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS && fs_alpha > 0.0)
        fs_alpha -= 0.005;

    if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS && fs_alpha < 1.0)
        fs_alpha += 0.005;

    if (glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
        model_rotation_along_x_axis += 0.1f;

    if (glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
        model_rotation_along_x_axis -= 0.1f;

    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
        view_translation_along_z_axis += 0.01f;

    if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
        view_translation_along_z_axis -= 0.01f;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindwHind(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow * pWindow = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (nullptr == pWindow)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);

    // std::string strVSPath = "shaders/vertex_shaders/VertexShader.vs";
    // std::string strFSPath = "shaders/fragment_shaders/FragmentShader.fs";
    // std::string strFSPathYellow = "shaders/fragment_shaders/FragmentShaderYellow.fs";

    // CShaderProgram program(strVSPath, strFSPath);
    // CShaderProgram programYellow(strVSPath, strFSPathYellow);

    std::string strVSPath = "shaders/vertex_shaders/vertex_shader_texture.vs";
    std::string strFSPath = "shaders/fragment_shaders/fragment_shader_texture.fs";

    CShaderProgram programTexture(strVSPath, strFSPath);

    // triangle
    // float vertices[] = {
    //     //     coords             colors
    //     -0.5f,  0.2f, 0.0f,   1.0f, 0.0f, 0.0f,
    //      0.5f,  0.2f, 0.0f,   0.0f, 1.0f, 0.0f,
    //      0.0f,  0.9f, 0.0f,   0.0f, 0.0f, 1.0f,
    //      0.3f,  0.3f, 0.0f,   1.0f, 0.0f, 0.0f,
    //      0.5f,  0.7f, 0.0f,   0.0f, 1.0f, 0.0f,
    //      0.3f,  0.7f, 0.0f,   0.0f, 0.0f, 1.0f
    // };

    // create buffer object and bind it to array buffer from context
    // unsigned int vertex_buffer;
    // glGenBuffers(1, &vertex_buffer);

    // unsigned int vertex_array;
    // glGenVertexArrays(1, &vertex_array);
    // glBindVertexArray(vertex_array);

    // glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
    // glEnableVertexAttribArray(1);

    // second triangle
    // float tri_vertices[] = {
    //     //   coords               colors
    //     -0.9f,  0.4f, 0.0f,    1.0f, 0.0f, 0.0f,
    //     -0.5f,  0.1f, 0.0f,    0.0f, 1.0f, 0.0f,
    //     -0.3f,  0.4f, 0.0f,    0.0f, 0.0f, 1.0f
    // };

    // unsigned int tri_vertex_buffer;
    // glGenBuffers(1, &tri_vertex_buffer);

    // unsigned int tri_vertex_array;
    // glGenVertexArrays(1, &tri_vertex_array);
    // glBindVertexArray(tri_vertex_array);

    // glBindBuffer(GL_ARRAY_BUFFER, tri_vertex_buffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(tri_vertices), tri_vertices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

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

    programTexture.Use();
    glUniform1i(glGetUniformLocation(programTexture.GetId(), "texture1"), 0);
    glUniform1i(glGetUniformLocation(programTexture.GetId(), "texture2"), 1);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f/ 600.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(programTexture.GetId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // rectangle
    float rect_vertices[] = {
        //     positions        colors             tex coords
        -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 
         0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int rect_vertex_buffer;
    glGenBuffers(1, &rect_vertex_buffer);

    unsigned int element_buffer;
    glGenBuffers(1, &element_buffer);

    unsigned int rect_vertex_array;
    glGenVertexArrays(1, &rect_vertex_array);
    glBindVertexArray(rect_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, rect_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    while (!glfwWindowShouldClose(pWindow))
    {
        process_input(pWindow);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // program.Use();

        // float time = glfwGetTime();
        // float green_value = std::sin(time) / 2.0f + 0.5f;
        // int vertexColorLocation = glGetUniformLocation(shader_program, "ourColor");
        // glUniform4f(vertexColorLocation, 0.0f, green_value, 0.0f, 1.0f);

        // glBindVertexArray(vertex_array);
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        // glBindVertexArray(tri_vertex_array);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // programYellow.Use();
        programTexture.Use();


        glm::mat4 model(1.0f);
        
        glm::mat4 view = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, view_translation_along_z_axis));
        view = glm::rotate(view, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(programTexture.GetId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(programTexture.GetId(), "view"), 1, GL_FALSE, glm::value_ptr(view));

        // glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -0.5f, 0.0f));    
        // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        glUniform1f(glGetUniformLocation(programTexture.GetId(), "value"), fs_alpha);
        // glUniformMatrix4fv(glGetUniformLocation(programTexture.GetId(), "transform"), 1, GL_FALSE, glm::value_ptr(trans));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(rect_vertex_array);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glm::mat4 trans2 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f));
        // float val = sin(glfwGetTime());
        // trans2 = glm::scale(trans2, glm::vec3(val, val, 1.0));
        // glUniformMatrix4fv(glGetUniformLocation(programTexture.GetId(), "transform"), 1, GL_FALSE, glm::value_ptr(trans2));

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}