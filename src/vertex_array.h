#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H
#include <vertex_buffer.h>
#include <vertex_buffer_layout.h>

namespace Engenius
{
class vertex_array
{
public:
    vertex_array();
    ~vertex_array();

    void bind() const;
    void add_buffer(const vertex_buffer& buffer, const vertex_buffer_layout& layout) const;
private:
    unsigned int id;
};
}; // end namespace Engenius
#endif // VERTEX_ARRAY_H