#include "Graphics/Math/Camera.h"

namespace SteelEngine {

    Camera::Camera(const Transform& transform) :
        m_Transform(transform)
    {
        m_Projection = glm::perspective(glm::radians(70.f), (float)1600 / (float)900, 0.1f, 1000.f);
    }

    Camera::~Camera()
    {

    }

    glm::mat4 Camera::GetView() const
    {
        return glm::lookAt(
            m_Transform.GetPosition(),
            m_Transform.GetPosition() + m_Transform.GetRotation().GetForward(),
            glm::normalize(m_Transform.GetRotation().GetUp())
        );
    }

    glm::mat4 Camera::GetProjection() const
    {
        return m_Projection;
    }

}