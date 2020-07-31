#pragma once

#include "Graphics/ITexture.h"

#include "Core/Type.h"

#include "Platform/Graphics/TextureParameter.h"

#include "functional"

namespace SteelEngine { namespace Utils { namespace Graphics {

// Tool for creating texture independent from selected renderer
// Create own implementation is needed for specified renderer
    class Texture
    {
        using ITexture = SteelEngine::Graphics::ITexture;
    private:

    public:
        Texture();
        ~Texture();

        static ITexture* Create();
        static ITexture* Create(const char* filename);
        static ITexture* Create(
            SteelEngine::Graphics::InternalFormat internalFormat,
            SteelEngine::Graphics::Format format,
            SteelEngine::Graphics::DataType tType,
            SteelEngine::Graphics::Filter filter,
            bool clamp
        );
        static ITexture* Create(
            uint32_t width,
            uint32_t height,
            SteelEngine::Graphics::InternalFormat internalFormat,
            SteelEngine::Graphics::Format format,
            SteelEngine::Graphics::DataType tType,
            SteelEngine::Graphics::Filter filter,
            bool clamp
        );
    };

}}}