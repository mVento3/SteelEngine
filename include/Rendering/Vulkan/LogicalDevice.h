#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

#include "Rendering/Vulkan/QueueFamilyIndices.h"

#include "set"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Renderer;
    class SwapChain;

    class LogicalDevice
    {
        friend class PhysicalDevice;
        friend class Renderer;
    private:
        VkDevice m_LogicalDevice;

        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;

    public:
        LogicalDevice();
        ~LogicalDevice();

        Result Create(Renderer* renderer);
        Result CreateSwapChain(const VkSwapchainCreateInfoKHR& createInfo, SwapChain* swapChain);

        inline VkDevice GetLogicalDevice() { return m_LogicalDevice; }
    };

}}}