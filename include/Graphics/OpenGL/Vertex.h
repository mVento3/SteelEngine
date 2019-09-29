#pragma once

#include "glm/glm.hpp"

namespace SteelEngine { namespace Graphics {

    struct Vertex
    {
        Vertex(const glm::vec3& position, const glm::vec2& texCoord, const glm::vec3& normal, const glm::vec3& tangent) :
            m_Position(position),
            m_TexCoord(texCoord),
            m_Normal(normal),
            m_Tangent(tangent)
        {

        }

        glm::vec3 m_Position;
        glm::vec2 m_TexCoord;
        glm::vec3 m_Normal;
        glm::vec3 m_Tangent;
    };

}}