#pragma once

#include "Graphics/IRenderer.h"
#include "Graphics/IFramebuffer.h"
#include "Graphics/ITexture.h"
#include "Graphics/FramebufferAttachment.h"

#include "RuntimeReflection/Macro.h"

#include "gl/glew.h"

namespace SteelEngine { namespace Graphics { namespace TestRenderer {

    SE_CLASS(
        IRenderer::RENDERER = "SteelEngine::Graphics::TestRenderer::Renderer"
    )
    class Framebuffer : public IFramebuffer
    {
    private:
        GLuint m_FBO;
        GLuint m_RBO;

        uint32_t m_Width;
        uint32_t m_Height;

        std::vector<FramebufferAttachment> m_Attachments;

    public:
        Framebuffer(uint32_t width, uint32_t height, std::vector<FramebufferAttachment> attachments);
        ~Framebuffer();

        void Setup(const RenderDevice* renderDevice) override;
        void Bind(const RenderDevice* renderDevice) const override;

        const std::vector<Graphics::FramebufferAttachment>& GetAttachments() const override { return m_Attachments; }
        void* GetFBO() const override { return (void*)&m_FBO; }
        void* GetRBO() const override { return (void*)&m_RBO; }
        uint32_t GetWidth() const override { return m_Width; }
        uint32_t GetHeight() const override { return m_Height; }
    };

}}}