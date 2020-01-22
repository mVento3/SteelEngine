#pragma once

#include "glm/glm.hpp"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    struct BaseLight
    {
        enum Uniforms
        {
            BASE_COLOR,
            INTENSITY,

            SIZE
        };

        glm::vec3 m_Color;
        float m_Intensity;
    };

}}}