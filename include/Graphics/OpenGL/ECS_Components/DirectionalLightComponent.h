#pragma once

#include "glm/glm.hpp"

#include "GL/glew.h"

#include "Graphics/OpenGL/Lights/BaseLight.h"

#include "Math/Transform.h"

#include "Core/Type.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    struct DirectionalLightComponent
    {
        enum Uniforms
        {
            ROTATION = BaseLight::SIZE,

            SIZE
        };

        DirectionalLightComponent()
        {
            m_Projection = glm::ortho(-100.f, 100.f, -100.f, 100.f, -100.f, 100.f);
            m_LightMatrix = glm::scale(glm::vec3(0, 0, 0));
        }

        glm::mat4 m_Projection;
        glm::mat4 m_LightMatrix;
        GLuint m_FBO;
        Type::uint32 m_Width;
        Type::uint32 m_Height;
        GLuint m_TextureID;
        Transform m_Transform;

        GLuint m_Uniforms[Uniforms::SIZE];
        BaseLight m_Base;
    };

}}}