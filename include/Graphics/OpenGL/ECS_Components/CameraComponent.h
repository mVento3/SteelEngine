#pragma once

#include "Math/Transform.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    struct CameraComponent
    {
        CameraComponent(float aspectRatio, const Transform& transform = Transform(), float fov = 70.f, float zNear = 0.1f, float zFar = 1000.f) :
            m_FOV(fov),
            m_Near(zNear),
            m_Far(zFar)
        {
            m_Projection = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
        }

        Transform m_Transform;
        glm::mat4 m_Projection;

        float m_FOV;
        float m_Near;
        float m_Far;
    };

}}}