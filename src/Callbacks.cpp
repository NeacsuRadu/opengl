#include <Callbacks.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Camera.h>

namespace Engenius
{
namespace Callbacks
{
float g_lastX = 400.0f;
float g_lastY = 300.0f;
float g_pitch = 0.0f;
float g_yaw = -90.0f;
float g_fov = 45.0f;

bool g_firstMouse = true;

void FramebufferSize(GLFWwindow * pWindow, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseMove(GLFWwindow * pWindow, double xpos, double ypos)
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

    Camera::Instance().UpdateDirection(g_yaw, g_pitch);
}

void MouseScroll(GLFWwindow * pWindow, double xoffset, double yoffset)
{
    Camera::Instance().UpdateFov(xoffset, yoffset);
}

}; // end namespace Callbacks
}; // end namespace Engenius;