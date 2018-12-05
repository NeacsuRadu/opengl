#include <Object.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Engenius
{

glm::mat4 Object::GetModelMatrix() const
{
    return glm::mat4(1.0f);
}

void Object::Update()
{
    m_velocity += m_acceleration;
    m_translation += m_velocity;
}

}; // end namespace Engenius
