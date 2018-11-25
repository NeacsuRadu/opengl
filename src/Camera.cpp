#include <Camera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Engenius
{
template<> Camera * Singleton<Camera>::m_pInstance = nullptr;

void Camera::UpdateDirection(float yaw, float pitch)
{
    glm::vec3 aux {
        glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
        glm::sin(glm::radians(pitch)),
        glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
    };
    m_direction = glm::normalize(aux);
} 

void Camera::UpdateFov(float /*xoffset*/, float yoffset)
{
    m_fov -= yoffset;
    if (m_fov <= 1.0f)
        m_fov = 1.0f;
    else if (m_fov >= 45.0f)
        m_fov = 45.0f;
}

void Camera::UpdatePositionFront()
{
    m_position += m_speed * m_direction;
}

void Camera::UpdatePositionBack()
{
    m_position -= m_speed * m_direction;
}

void Camera::UpdatePositionLeft()
{
    m_position -= m_speed * glm::normalize(glm::cross(m_direction, m_worldUp));
}

void Camera::UpdatePositionRight()
{
    m_position += m_speed * glm::normalize(glm::cross(m_direction, m_worldUp));
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_direction, m_worldUp);
}
}; // end namespace Engenius