#pragma once

#include "GL/glew.h"

#include "Math/Transform.h"

namespace SteelEngine { namespace Graphics { namespace Renderer3D {

    struct VAO_Component
    {
        GLuint m_VAO;
        uint32_t m_DrawCount;
        glm::mat4 m_Transform;
    };

}}}