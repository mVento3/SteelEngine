#pragma once

#include "glm/glm.hpp"

namespace SteelEngine { namespace Graphics {

    struct Vertex
    {
        Vertex(const glm::vec3& position) :
            m_Position(position)
        {

        }

        glm::vec3 m_Position;
    };

}}