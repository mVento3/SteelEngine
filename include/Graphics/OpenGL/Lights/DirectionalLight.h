#pragma once

#include "Graphics/OpenGL/Lights/BaseLight.h"
#include "Graphics/OpenGL/Lights/Light.h"

#include "Graphics/OpenGL/Shader.h"
#include "Graphics/OpenGL/ShadowInfo.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    class DirectionalLight : public Light
    {
    public:
        enum Uniforms
        {
            BASE_LIGHT = BaseLight::SIZE,
            ROTATION,

            SIZE
        };

    private:
        BaseLight m_BaseLight;
        Quaternion m_Rotation;

        GLuint m_Uniforms[Uniforms::SIZE];

        void Init(const Shader& shader) override;

    public:
        DirectionalLight(const BaseLight& baseLight, const Quaternion& rotation);
        ~DirectionalLight();

        void Update(Transform& transform) override;
        void Update(const Shader& shader) override;
    };

}}}