#pragma once

#include "Graphics/OpenGL/Lights/BaseLight.h"
#include "Graphics/OpenGL/Lights/Light.h"

#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/ShadowInfo.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    struct Attenuation
    {
        float m_Constant;
        float m_Linear;
        float m_Exponent;
    };

    struct PointLight
    {
        PointLight(const BaseLight& baseLight, const Attenuation& attenuation, const glm::vec3& position, float range = 0) :
            m_Base(baseLight),
            m_Attenuation(attenuation),
            m_Position(position),
            m_Range(range)
        {
            if(range == 0)
            {
                float a = attenuation.m_Exponent;
                float b = attenuation.m_Linear;
                float c = attenuation.m_Constant - 256 * baseLight.m_Intensity * glm::max(glm::max(baseLight.m_Color.x, baseLight.m_Color.y), baseLight.m_Color.z);

                m_Range = (-b + sqrtf(b * b - 4 * a * c)) / (2 * a);
            }
        }

        BaseLight m_Base;
        Attenuation m_Attenuation;
        glm::vec3 m_Position;
        float m_Range;
    };

    class SpotLight : public Light
    {
    private:
        PointLight m_PointLight;
        Quaternion m_Rotation;
        float m_Cutoff;

    public:
        SpotLight(const PointLight& pointLight, const Quaternion& rotation, float viewAngle);
        ~SpotLight();

        void Update(Transform& transform) override;
        void Update(const Shader& shader) override;

        inline void SetRotation(const Quaternion& rotation) { m_Rotation = rotation; }

        inline PointLight& GetPointLight() { return m_PointLight; }
    };

}}}