#pragma once

#include "vector"

#include "Vulkan/vulkan.h"

#include "Graphics/Vulkan/PresentationLayer/Surface.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR        m_Capabilities;
        std::vector<VkSurfaceFormatKHR> m_Formats;
        std::vector<VkPresentModeKHR>   m_PresentModes;

        static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
        {
            SwapChainSupportDetails details;

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.m_Capabilities);

            uint32_t formatCount;

            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

            if(formatCount != 0)
            {
                details.m_Formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.m_Formats.data());
            }

            uint32_t presentModeCount;

            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

            if(presentModeCount != 0)
            {
                details.m_PresentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.m_PresentModes.data());
            }

            return details;
        }
    };

}}}