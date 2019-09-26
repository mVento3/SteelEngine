#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

#include "Window/IWindow.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Renderer;

    class Surface
    {
        friend struct QueueFamilyIndices;
        friend class PhysicalDevice;
        friend struct SwapChainSupportDetails;
        friend class SwapChain;
        friend class Renderer;
        friend class CommandPool;

        friend class Device;
        friend class LogicalDevice;
    private:
        VkSurfaceKHR m_Surface;

    public:
        Surface();
        ~Surface();

        Result Create(Renderer* renderer, IWindow* window);
        void Cleanup(Renderer* renderer);

        static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    };

}}}