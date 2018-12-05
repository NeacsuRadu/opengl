#include <vertex_buffer.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engenius
{
vertex_buffer::vertex_buffer(const void * data, unsigned int size)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

vertex_buffer::~vertex_buffer()
{
    glDeleteBuffers(1, &id);
}

void vertex_buffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}
};

