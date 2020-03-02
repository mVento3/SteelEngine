#pragma once

#include "Math/Transform.h"

namespace SteelEngine {

    class Camera
    {
    private:
        Transform m_Transform;
        glm::mat4 m_Projection;

    public:
        Camera(const Transform& transform, float aspect, float fov = 70.f, float near = 0.1f, float far = 1000.f);
        ~Camera();

        float m_Fov;
        float m_Near;
        float m_Far;

        glm::mat4 GetView() const;
        glm::mat4 GetProjection() const;

        inline void SetProjection(const glm::mat4& projection) { m_Projection = projection; }

        inline Transform& GetTransform() { return m_Transform; }
        inline const Transform& GetTransform() const { return m_Transform; }
    };

}