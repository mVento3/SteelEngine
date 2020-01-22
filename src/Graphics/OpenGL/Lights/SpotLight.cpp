#include "Graphics/OpenGL/Lights/SpotLight.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    void SpotLight::Init(const Shader& shader)
    {
        m_Uniforms[BaseLight::BASE_COLOR] = shader.GetUniformLocation("spotLight.pointLight.base.color");
        m_Uniforms[BaseLight::INTENSITY] = shader.GetUniformLocation("spotLight.pointLight.base.intensity");
        m_Uniforms[Attenuation::CONSTANT] = shader.GetUniformLocation("spotLight.pointLight.attenuation.constant");
        m_Uniforms[Attenuation::LINEAR] = shader.GetUniformLocation("spotLight.pointLight.attenuation.linear");
        m_Uniforms[Attenuation::EXPONENT] = shader.GetUniformLocation("spotLight.pointLight.attenuation.exponent");
        m_Uniforms[PointLight::POSITION] = shader.GetUniformLocation("spotLight.pointLight.position");
        m_Uniforms[PointLight::RANGE] = shader.GetUniformLocation("spotLight.pointLight.range");
        m_Uniforms[SpotLight::DIRECTION] = shader.GetUniformLocation("spotLight.direction");
        m_Uniforms[SpotLight::CUTOFF] = shader.GetUniformLocation("spotLight.cutoff");
    }

    SpotLight::SpotLight(const PointLight& pointLight, const Quaternion& rotation, float viewAngle) :
        m_PointLight(pointLight),
        m_Rotation(rotation),
        m_Cutoff(cosf(viewAngle / 2))
    {
        m_ShadowInfo = new ShadowInfo(glm::perspective(viewAngle, 1.f, 0.1f, pointLight.m_Range));
    }

    SpotLight::~SpotLight()
    {

    }

    void SpotLight::Update(Transform& transform)
    {
        transform.SetPosition(m_PointLight.m_Position);
        transform.SetRotation(m_Rotation);
    }

    void SpotLight::Update(const Shader& shader)
    {
        shader.SetVec3(m_Uniforms[BaseLight::BASE_COLOR], m_PointLight.m_Base.m_Color);
        shader.SetFloat(m_Uniforms[BaseLight::INTENSITY], m_PointLight.m_Base.m_Intensity);
        shader.SetFloat(m_Uniforms[Attenuation::CONSTANT], m_PointLight.m_Attenuation.m_Constant);
        shader.SetFloat(m_Uniforms[Attenuation::LINEAR], m_PointLight.m_Attenuation.m_Linear);
        shader.SetFloat(m_Uniforms[Attenuation::EXPONENT], m_PointLight.m_Attenuation.m_Exponent);
        shader.SetVec3(m_Uniforms[PointLight::POSITION], m_PointLight.m_Position);
        shader.SetFloat(m_Uniforms[PointLight::RANGE], m_PointLight.m_Range);
        shader.SetVec3(m_Uniforms[SpotLight::DIRECTION], m_Rotation.GetForward());
        shader.SetFloat(m_Uniforms[SpotLight::CUTOFF], m_Cutoff);
    }

}}}