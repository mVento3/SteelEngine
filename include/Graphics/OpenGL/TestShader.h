#pragma once

#include "Graphics/OpenGL/Shader.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class TestShader : public Shader
    {
    public:
        enum Uniforms
        {
            AMBIENT_LIGHT,
            BASE_COLOR,

            EYE_POSITION,

            SPECULAR_INTENSITY,
            SPECULAR_POWER,

            DISP_MAP_SCALE,
            DISP_MAP_OFFSET,

            SIZE
        };

    private:
        GLuint m_Uniforms[Uniforms::SIZE];

        void InitCustom() override;

    public:
        TestShader();
        ~TestShader();

        glm::vec3 m_AmbientLight;
        glm::vec3 m_BaseColor;
        float m_SpecularIntensity;
        float m_SpecularPower;
        float m_DispMapScale;
        float m_DispMapOffset;

        void UpdateCustom(const Camera& camera) override;
    };

}}}