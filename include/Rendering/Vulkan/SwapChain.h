#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

#include "Rendering/Vulkan/SwapChainSupportDetails.h"

#include "vector"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class PhysicalDevice;
    class Surface;
    class LogicalDevice;

    class SwapChain
    {
        friend class LogicalDevice;
        friend class GraphicsPipeline;
        friend class RenderPass;
        friend class Framebuffer;
        friend class CommandPool;
        friend class Renderer;
    private:
        VkSwapchainKHR              m_SwapChain;
        std::vector<VkImage>        m_SwapChainImages;
        VkFormat                    m_SwapChainImageFormat;
        VkExtent2D                  m_SwapChainExtent;
        std::vector<VkImageView>    m_SwapChainImageViews;

        Result CreateImageViews(const LogicalDevice& logicalDevice);

    public:
        SwapChain();
        ~SwapChain();

        Result Create(
            const PhysicalDevice& physicalDevice,
            const LogicalDevice& logicalDevice,
            const Surface& surface
        );
        void Cleanup(const LogicalDevice& logicalDevice);
    };

}}}