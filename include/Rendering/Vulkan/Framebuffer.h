#pragma once

#include "Core/Result.h"

#include "Vulkan/vulkan.h"

#include "vector"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Renderer;

    class Framebuffer
    {
        friend class CommandPool;
    private:
        std::vector<VkFramebuffer> m_SwapChainFramebuffers;

    public:
        Framebuffer();
        ~Framebuffer();

        Result Create(Renderer* renderer);
    };

}}}