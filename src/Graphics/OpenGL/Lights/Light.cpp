#include "Graphics/OpenGL/Lights/Light.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    Light::Light()
    {
        m_ShadowMapTexture = new Texture(GL_RG32F, GL_RGBA, GL_FLOAT, GL_LINEAR, true);
        m_ShadowFramebuffer = new Framebuffer(4096, 4096,
        {
            new Framebuffer::Attachment(m_ShadowMapTexture, GL_COLOR_ATTACHMENT0)
        });
    }

    Light::~Light()
    {

    }

    void Light::Setup(const Shader& shader)
    {
        m_ShadowFramebuffer->Setup();

        Init(shader);
    }

}}}