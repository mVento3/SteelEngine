#pragma once

#include "GL/glew.h"

#include "glm/glm.hpp"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    struct ShadowShaderComponent
    {
        enum Uniforms
        {
            MODEL_U,
            PROJECTION_U,
            VIEW_U,

            SIZE
        };

        ShadowShaderComponent()
        {

        }

        GLuint m_Uniforms[SIZE];
        GLuint m_ShaderID;
    };

}}}