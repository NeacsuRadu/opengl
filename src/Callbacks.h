#ifndef CALLBACKS_H
#define CALLBACKS_H
class GLFWwindow;

namespace Engenius
{
namespace Callbacks
{
// Called when the window is resized
void FramebufferSize(GLFWwindow * pWindow, int width, int height);

// Called when the mouse is moved
void MouseMove(GLFWwindow * pWindow, double xpos, double ypos);

// Called when scroll button is moved
void MouseScroll(GLFWwindow * pWindow, double xoffset, double yoffset);

}; // end namespace Callbacks
}; // end namespace Engenius
#endif // CALLBACKS_H