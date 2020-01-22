#include "Graphics/OpenGL/Lights/DirectionalLight.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    void DirectionalLight::Init(const Shader& shader)
    {
        m_Uniforms[BaseLight::BASE_COLOR] = shader.GetUniformLocation("directionalLight.base.color");
        m_Uniforms[BaseLight::INTENSITY] = shader.GetUniformLocation("directionalLight.base.intensity");
        m_Uniforms[DirectionalLight::ROTATION] = shader.GetUniformLocation("directionalLight.direction");
    }

    DirectionalLight::DirectionalLight(const BaseLight& baseLight, const Quaternion& rotation) :
        m_BaseLight(baseLight),
        m_Rotation(rotation)
    {
        m_ShadowInfo = new ShadowInfo(glm::ortho(-100.f, 100.f, -100.f, 100.f, -100.f, 100.f));
    }

    DirectionalLight::~DirectionalLight()
    {

    }

    void DirectionalLight::Update(Transform& transform)
    {
        transform.SetPosition(glm::vec3(0, 0, 0));
        transform.SetRotation(m_Rotation * glm::quat(glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0))));
    }    

    void DirectionalLight::Update(const Shader& shader)
    {
        shader.SetVec3(m_Uniforms[BaseLight::BASE_COLOR], m_BaseLight.m_Color);
        shader.SetFloat(m_Uniforms[BaseLight::INTENSITY], m_BaseLight.m_Intensity);
        shader.SetVec3(m_Uniforms[DirectionalLight::ROTATION], m_Rotation.GetForward());
    }

}}}