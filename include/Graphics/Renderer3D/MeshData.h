#pragma once

#include "GL/glew.h"

#include "Core/Type.h"

#include "Math/Transform.h"

namespace SteelEngine { namespace Graphics { namespace Renderer3D {

// This struct is only to keep mesh data, it is not used to render. For render we are using VAO_Component
    struct MeshData
    {
        GLuint      m_VAO;
        GLuint*     m_VBOs;
        uint32_t    m_SizeOfVBOs;
        uint32_t    m_DrawCount;
        Transform   m_ObjectTransform;
    };

}}}