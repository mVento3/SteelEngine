#pragma once

#include "Graphics/OpenGL/Transform.h"

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

        inline Transform& GetTransform() { return m_Transform; }
    };

}