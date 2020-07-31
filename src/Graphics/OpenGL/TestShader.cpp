#include "Graphics/OpenGL/TestShader.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    void TestShader::InitCustom()
    {
        m_Uniforms[Uniforms::AMBIENT_LIGHT] = GetUniformLocation("ambientLight");
        m_Uniforms[Uniforms::BASE_COLOR] = GetUniformLocation("baseColor");
        m_Uniforms[Uniforms::EYE_POSITION] = GetUniformLocation("eyePosition");
        m_Uniforms[Uniforms::SPECULAR_INTENSITY] = GetUniformLocation("specularIntensity");
        m_Uniforms[Uniforms::SPECULAR_POWER] = GetUniformLocation("specularPower");
        m_Uniforms[Uniforms::DISP_MAP_SCALE] = GetUniformLocation("dispMapScale");
        m_Uniforms[Uniforms::DISP_MAP_OFFSET] = GetUniformLocation("dispMapOffset");
    }

    TestShader::TestShader() :
        Shader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/OpenGL/gBuffer")
    {

    }

    TestShader::~TestShader()
    {

    }

    void TestShader::UpdateCustom(const Camera& camera)
    {
        SetVec3(m_Uniforms[Uniforms::EYE_POSITION], camera.GetTransform().GetPosition());

        SetVec3(m_Uniforms[Uniforms::AMBIENT_LIGHT], m_AmbientLight);
        SetVec3(m_Uniforms[Uniforms::BASE_COLOR], m_BaseColor);

        SetFloat(m_Uniforms[Uniforms::SPECULAR_INTENSITY], m_SpecularIntensity);
        SetFloat(m_Uniforms[Uniforms::SPECULAR_POWER], m_SpecularPower);

        SetFloat(m_Uniforms[Uniforms::DISP_MAP_SCALE], m_DispMapScale);
        SetFloat(m_Uniforms[Uniforms::DISP_MAP_OFFSET], m_DispMapOffset);
    }

}}}