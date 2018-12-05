#include <index_buffer.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engenius
{
index_buffer::index_buffer(const unsigned int * data, unsigned int count)
    : count (count)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

index_buffer::~index_buffer()
{
    glDeleteBuffers(1, &id);
}

void index_buffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}
};