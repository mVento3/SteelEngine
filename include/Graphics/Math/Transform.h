#pragma once

#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtx\quaternion.hpp"

#include "Graphics/Math/Quaternion.h"

namespace SteelEngine {

    class Transform
    {
    private:
        glm::vec3 m_Position;
        Quaternion m_Rotation;
        glm::vec3 m_Scale;

    public:
        Transform(const glm::vec3& position = glm::vec3(0, 0, 0), const Quaternion& rotation = Quaternion(0, 0, 0, 1), const glm::vec3& scale = glm::vec3(1, 1, 1));
        ~Transform();

        glm::mat4 GetModel() const;

        inline const glm::vec3& GetPosition() { return m_Position; }
        inline const Quaternion& GetRotation() { return m_Rotation; }
        inline const glm::vec3& GetScale() { return m_Scale; }

        inline const glm::vec3& GetPosition() const { return m_Position; }
        inline const Quaternion& GetRotation() const { return m_Rotation; }
        inline const glm::vec3& GetScale() const { return m_Scale; }

        inline void SetPosition(const glm::vec3& position) { m_Position = position; }
        inline void SetRotation(const Quaternion& rotation) { m_Rotation = rotation; }
        inline void SetScale(const glm::vec3& scale) { m_Scale = scale; }
    };

}