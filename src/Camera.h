#ifndef CAMERA_H
#define CAMERA_H
#include <Singleton.h>
#include <glm/glm.hpp>

namespace Engenius
{
class Camera:
    public Singleton<Camera>
{
public:
    void UpdateDirection(float yaw, float pitch);
    void UpdateFov(float xoffset, float yoffset);
    void UpdatePositionFront();
    void UpdatePositionBack();
    void UpdatePositionLeft();
    void UpdatePositionRight();
    inline void SetDeltaTime(float dTime) { m_deltaTime = dTime; }

    inline glm::vec3 GetPosition() const { return m_position; }
    inline glm::vec3 GetDirection() const { return m_direction; }
    inline float GetFov() const { return m_fov; }
    glm::mat4 GetViewMatrix() const; 
private:
    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_worldUp = glm::vec3(0.0f, 1.0f, 0.0);

    float m_speed = 0.5f;
    float m_fov = 45.0f;
    float m_deltaTime = 0.0f;
};

}; // end namespace Engenius

#endif // CAMERA_H