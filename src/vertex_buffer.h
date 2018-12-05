#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

namespace Engenius
{
class vertex_buffer 
{
public:
    vertex_buffer(const void * data, unsigned int size);
    ~vertex_buffer();
    
    void bind() const;
private:
    unsigned int id;
};
}; // namespace Engenius
#endif // VERTEX_BUFFER_H