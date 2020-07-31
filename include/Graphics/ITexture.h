#pragma once

#include "Core/Type.h"

#include "Platform/Graphics/ITexture.h"

namespace SteelEngine { namespace Graphics {

    class RenderDevice;

    struct ITexture
    {
    // All those options should be tested and corrected

    // TODO: To remove
        virtual void Setup() { }
        virtual void Setup(const RenderDevice* renderDevice) { }

    // TODO: Temp, should be pure virtual
        virtual void SetWidth(uint32_t width) { }
        virtual void SetHeight(uint32_t height) { }

        virtual Platform::Graphics::ITexture** GetTextureID_Ptr() const { return 0; }
        virtual unsigned int GetTextureID() const { return 0; }
        virtual unsigned int GetTextureWidth() const = 0;
        virtual unsigned int GetTextureHeight() const = 0;
    };

}}