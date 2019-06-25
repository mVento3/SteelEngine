#pragma once

#include "Core/Result.h"

#include "Vulkan/vulkan.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class SwapChain;
    class LogicalDevice;

    class RenderPass
    {
    private:
        VkRenderPass m_RenderPass;

    public:
        RenderPass();
        ~RenderPass();

        Result Create(const LogicalDevice& logicalDevice, const SwapChain& swapChain);
        void Cleanup(const LogicalDevice& logicalDevice);

        inline VkRenderPass GetRenderPass() const { return m_RenderPass; }
    };

}}}