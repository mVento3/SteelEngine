#pragma once

#include "glm/glm.hpp"

#include "glm/gtc/quaternion.hpp"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace SteelEngine {

    class Transform
    {
    private:
        glm::vec3 m_Position;
        glm::quat m_Rotation;
        glm::vec3 m_Scale;

    public:
        Transform(const glm::vec3& position = glm::vec3(0, 0, 0), const glm::quat& rotation = glm::quat(1, 0, 0, 0), const glm::vec3& scale = glm::vec3(1, 1, 1));
        ~Transform();

        glm::mat4 GetModel() const;

        inline const glm::vec3& GetPosition() { return m_Position; }
        inline const glm::quat& GetRotation() { return m_Rotation; }
        inline const glm::vec3& GetScale() { return m_Scale; }

        inline glm::vec3 GetPosition() const { return m_Position; }
        inline glm::quat GetRotation() const { return m_Rotation; }
        inline glm::vec3 GetScale() const { return m_Scale; }

        inline void SetPosition(const glm::vec3& position) { m_Position = position; }
        inline void SetRotation(const glm::quat& rotation) { m_Rotation = rotation; }
        inline void SetScale(const glm::vec3& scale) { m_Scale = scale; }
    };

}