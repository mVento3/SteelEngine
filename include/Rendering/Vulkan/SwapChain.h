#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

#include "Rendering/Vulkan/SwapChainSupportDetails.h"

#include "vector"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Renderer;

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

        Result CreateImageViews(Renderer* renderer);

    public:
        SwapChain();
        ~SwapChain();

        Result Create(Renderer* renderer);
    };

}}}