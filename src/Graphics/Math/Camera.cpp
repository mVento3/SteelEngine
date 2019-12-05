#include "Graphics/Math/Camera.h"

namespace SteelEngine {

    Camera::Camera(const Transform& transform, float aspect, float fov, float near, float far) :
        m_Transform(transform)
    {
        m_Projection = glm::perspective(glm::radians(fov), aspect, near, far);
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