#ifndef OBJECT_H
#define OBJECT_H
#include <glm/glm.hpp>

namespace Engenius
{
class Object
{
public:
    inline void SetAcceleration(const glm::vec3& acc) { m_acceleration = acc; }
    void Update();

    glm::mat4 GetModelMatrix() const;
private:
    glm::vec3 m_velocity     = glm::vec3(0.0f);
    glm::vec3 m_acceleration = glm::vec3(0.0f);
    glm::vec3 m_translation  = glm::vec3(1.0f);
};
}; // end namespace Engeniu
#endif // OBJECT_H