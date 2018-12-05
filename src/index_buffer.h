#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H
namespace Engenius
{
class index_buffer 
{
public:
    index_buffer(const unsigned int * data, unsigned int count);
    ~index_buffer();

    void bind() const;
private:
    unsigned int id;
    unsigned int count;
};
}; // namespace Engeius
#endif // INDEX_BUFFER_H