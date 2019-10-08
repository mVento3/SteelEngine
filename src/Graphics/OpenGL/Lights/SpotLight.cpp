#include "Graphics/OpenGL/Lights/SpotLight.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

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
        shader.SetVec3("spotLight.pointLight.base.color", m_PointLight.m_Base.m_Color);
        shader.SetFloat("spotLight.pointLight.base.intensity", m_PointLight.m_Base.m_Intensity);
        shader.SetFloat("spotLight.pointLight.attenuation.constant", m_PointLight.m_Attenuation.m_Constant);
        shader.SetFloat("spotLight.pointLight.attenuation.linear", m_PointLight.m_Attenuation.m_Linear);
        shader.SetFloat("spotLight.pointLight.attenuation.exponent", m_PointLight.m_Attenuation.m_Exponent);
        shader.SetVec3("spotLight.pointLight.position", m_PointLight.m_Position);
        shader.SetFloat("spotLight.pointLight.range", m_PointLight.m_Range);
        shader.SetVec3("spotLight.direction", m_Rotation.GetForward());
        shader.SetFloat("spotLight.cutoff", m_Cutoff);
    }

}}}