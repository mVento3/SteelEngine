#pragma once

#include "Graphics/FramebufferAttachment.h"

#include "vector"

#include "Platform/Graphics/RenderDevice.h"

namespace SteelEngine { namespace Graphics {

    struct IFramebuffer
    {
        virtual void Setup(const RenderDevice* renderDevice) { }
        virtual void Bind(const RenderDevice* renderDevice) const { }

        virtual const std::vector<Graphics::FramebufferAttachment>& GetAttachments() const = 0;
        virtual void* GetFBO() const = 0;
        virtual void* GetRBO() const = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
    };

}}