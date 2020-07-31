#pragma once

#include "GL/glew.h"

namespace SteelEngine { namespace Graphics { namespace Renderer3D {

    struct TextureComponent
    {
        GLuint m_TextureID;
        uint32_t m_TextureTarget;
        uint32_t m_TextureUnit;
    };

}}}