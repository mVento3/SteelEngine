#include "Graphics/OpenGL/Lights/DirectionalLight.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

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
        shader.SetVec3("directionalLight.base.color", m_BaseLight.m_Color);
        shader.SetFloat("directionalLight.base.intensity", m_BaseLight.m_Intensity);
        shader.SetVec3("directionalLight.direction", m_Rotation.GetForward());
    }

}}}