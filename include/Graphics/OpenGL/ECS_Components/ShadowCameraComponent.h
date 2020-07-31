#pragma once

#include "Math/Transform.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    struct ShadowCameraComponent
    {
        ShadowCameraComponent()
        {

        }

        Transform m_Transform;
        glm::mat4 m_Projection;

        float m_Fov;
        float m_Near;
        float m_Far;
    };

}}}