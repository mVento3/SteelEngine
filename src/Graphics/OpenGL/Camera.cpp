#include "Graphics/OpenGL/Camera.h"

namespace SteelEngine {

    Camera::Camera(const Transform& transform) :
        m_Transform(transform)
    {
        m_Projection = glm::perspective(70.f, (float)1920 / (float)1080, 0.1f, 1000.f);

        glm::vec3 forward = m_Transform.GetRotation() * glm::vec3(1, 0, 0);
    }

    Camera::~Camera()
    {

    }

    glm::mat4 Camera::GetView() const
    {
        return glm::lookAt(
            m_Transform.GetPosition(),
            m_Transform.GetPosition() + m_Transform.GetRotation() * glm::vec3(0, 0, 1),
            m_Transform.GetRotation() * glm::vec3(0, 1, 0)
        );
    }

    glm::mat4 Camera::GetProjection() const
    {
        return m_Projection;
    }

}