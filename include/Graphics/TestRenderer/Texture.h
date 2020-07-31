#pragma once

#include "Graphics/ITexture.h"
#include "Graphics/IRenderer.h"

#include "Platform/Graphics/TextureParameter.h"
#include "Platform/Graphics/RenderDevice.h"

#include "gl/glew.h"

namespace SteelEngine { namespace Graphics { namespace TestRenderer {

    SE_CLASS(
        IRenderer::RENDERER = "SteelEngine::Graphics::TestRenderer::Renderer"
    )
    class Texture : public ITexture
    {
    private:
        int m_Width;
        int m_Height;
        int m_Channels;
        unsigned char* m_Data;

        Platform::Graphics::ITexture* m_ID;

        InternalFormat m_InternalFormat;
        Format m_Format;
        DataType m_Type;
        Filter m_Filter;

        bool m_Clamp;

    public:
        Texture();
        Texture(const char* filename);
        Texture(
            InternalFormat internalFormat,
            Format format,
            DataType type,
            Filter filter,
            bool clamp
        );
        Texture(
            uint32_t width,
            uint32_t height,
            InternalFormat internalFormat,
            Format format,
            DataType type,
            Filter filter,
            bool clamp
        );
        ~Texture();

        void Setup(const RenderDevice* renderDevice) override;

        void SetWidth(uint32_t width) override;
        void SetHeight(uint32_t height) override;

        Platform::Graphics::ITexture** GetTextureID_Ptr() const override;
        unsigned int GetTextureWidth() const override;
        unsigned int GetTextureHeight() const override;
    };

}}}