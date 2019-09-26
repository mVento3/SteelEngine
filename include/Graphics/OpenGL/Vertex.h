#pragma once

#include "glm/glm.hpp"

namespace SteelEngine { namespace Graphics {

    struct Vertex
    {
        Vertex(const glm::vec3& position, const glm::vec2& texCoord) :
            m_Position(position),
            m_TexCoord(texCoord)
        {

        }

        glm::vec3 m_Position;
        glm::vec2 m_TexCoord;
    };

}}