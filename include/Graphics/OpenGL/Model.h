#pragma once

#include "Math/Transform.h"

#include "Graphics/OpenGL/Mesh.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    struct Model
    {
        Model(const Transform& transform, Mesh* mesh, const glm::vec3& color) :
            m_Transform(transform),
            m_Mesh(mesh),
            m_Color(color)
        {

        }

        Transform m_Transform;
        Mesh* m_Mesh;
        glm::vec3 m_Color;
    };

}}}