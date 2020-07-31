#pragma once

#include "glm/glm.hpp"

namespace SteelEngine { namespace Graphics { namespace Renderer2D {

    struct Vertex
    {
        glm::vec3 m_Position;
        glm::vec4 m_Color;
        glm::vec2 m_TexCoord;
        float m_TexID;
    };

}}}