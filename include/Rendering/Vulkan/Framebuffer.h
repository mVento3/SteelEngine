#pragma once

#include "Core/Result.h"

#include "Vulkan/vulkan.h"

#include "vector"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class SwapChain;
    class LogicalDevice;
    class RenderPass;

    class Framebuffer
    {
        friend class CommandPool;
    private:
        std::vector<VkFramebuffer> m_SwapChainFramebuffers;

    public:
        Framebuffer();
        ~Framebuffer();

        Result Create(
            const LogicalDevice& logicalDevice,
            const SwapChain& swapChain,
            const RenderPass& renderPass
        );
        void Cleanup(const LogicalDevice& logicalDevice);
    };

}}}