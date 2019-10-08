#pragma once

#include "Graphics/Math/Transform.h"

namespace SteelEngine {

    class Camera
    {
    private:
        Transform m_Transform;
        glm::mat4 m_Projection;

    public:
        Camera(const Transform& transform);
        ~Camera();

        glm::mat4 GetView() const;
        glm::mat4 GetProjection() const;

        inline void SetProjection(const glm::mat4& projection) { m_Projection = projection; }

        inline Transform& GetTransform() { return m_Transform; }
    };

}