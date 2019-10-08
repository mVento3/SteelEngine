#pragma once

#include "glm/glm.hpp"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    struct BaseLight
    {
        glm::vec3 m_Color;
        float m_Intensity;
    };

}}}