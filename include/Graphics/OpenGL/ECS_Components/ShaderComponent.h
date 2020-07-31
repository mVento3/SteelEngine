#pragma once

#include "GL/glew.h"

#include "glm/glm.hpp"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    struct ShaderComponent
    {
        enum Uniforms
        {
            MODEL_U,
            PROJECTION_U,
            VIEW_U,

            LIGHT_MATRIX_U,
            LIGHT_MATRIX2_U,

            AMBIENT_LIGHT,
            BASE_COLOR,

            EYE_POSITION,

            SPECULAR_INTENSITY,
            SPECULAR_POWER,

            DISP_MAP_SCALE,
            DISP_MAP_OFFSET,

            SIZE
        };

        ShaderComponent()
        {

        }

        GLuint m_Uniforms[SIZE];
        GLuint m_ShaderID;

        glm::vec3 m_BaseColor;
        glm::vec3 m_AmbientLight;
        float m_SpecularIntensity;
        float m_SpecularPower;
        float m_DispMapScale;
        float m_DispMapOffset;
    };

}}}