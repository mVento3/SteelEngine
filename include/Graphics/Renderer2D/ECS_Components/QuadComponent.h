#pragma once

#include "Graphics/Renderer2D/Vertex.h"

namespace SteelEngine { namespace Graphics { namespace Renderer2D {

    struct QuadComponent
    {
        Vertex m_Vertices[4];
        glm::mat4 m_Transform;
    };

}}}