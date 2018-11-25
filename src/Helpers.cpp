#include <Helpers.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Callbacks.h>

#include <iostream>

namespace Engenius
{
GLFWwindow *  InitGl(unsigned int width, unsigned int height)
{
    std::cout << "Enter" << std::endl;
    glfwInit();std::cout << "0";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);std::cout << "0";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);std::cout << "0";
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);std::cout << "0";

    GLFWwindow * pWindow = nullptr;
    do
    {
        std::cout << "0";
        pWindow = glfwCreateWindow(width, height, "Widow", nullptr, nullptr);
        if (!pWindow)
        {
            std::cout << "Failed to create window" << std::endl;
            break;
        }
        std::cout << "1";
        glfwMakeContextCurrent(pWindow);
        glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to init GLAD." << std::endl;
            glfwTerminate();
            return 0;
        }


        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, width, height);

        SetCallbacks(pWindow);

    } while (false);

    glfwTerminate();
    return pWindow;
}

void SetCallbacks(GLFWwindow * pWindow)
{
    glfwSetCursorPosCallback(pWindow, Callbacks::MouseMove);
    glfwSetScrollCallback(pWindow, Callbacks::MouseScroll);
    glfwSetFramebufferSizeCallback(pWindow, Callbacks::FramebufferSize);
}
}; // end namespace Engenius