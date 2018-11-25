#ifndef HELPERS_H
#define HELPERS_H

class GLFWwindow;
namespace Engenius
{
GLFWwindow * InitGl(unsigned int width, unsigned int height);
void SetCallbacks(GLFWwindow * window);
}; // end namespace Engenius
#endif // HELPERS_H