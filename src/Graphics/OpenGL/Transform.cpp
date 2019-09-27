#include "Graphics/OpenGL/Transform.h"

namespace SteelEngine {

    Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) :
        m_Position(position),
        m_Rotation(rotation),
        m_Scale(scale)
    {

    }

    Transform::~Transform()
    {

    }

    glm::mat4 Transform::GetModel() const
    {
        return glm::translate(m_Position) * glm::toMat4(m_Rotation) * glm::scale(m_Scale);
    }

}