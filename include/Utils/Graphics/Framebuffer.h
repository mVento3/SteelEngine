#pragma once

#include "Graphics/IFramebuffer.h"
#include "Graphics/FramebufferAttachment.h"

#include "vector"

namespace SteelEngine { namespace Utils { namespace Graphics {

    class Framebuffer
    {
        using IFramebuffer = SteelEngine::Graphics::IFramebuffer;
        using FramebufferAttachment = SteelEngine::Graphics::FramebufferAttachment;
    private:

    public:
        Framebuffer();
        ~Framebuffer();

        static IFramebuffer* Create(uint32_t width, uint32_t height, std::vector<FramebufferAttachment> attachments);
    };

}}}