#include "Rendering/Vulkan/Surface.h"

#include "Rendering/Vulkan/Renderer.h"

#include "algorithm"

#undef max
#undef min

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    Surface::Surface()
    {

    }

    Surface::~Surface()
    {

    }

    Result Surface::Create(Renderer* renderer, Interface::IWindow* window)
    {
        window->CreateVulkanSurface(renderer->m_Instance, (void**)&m_Surface);

        return SE_TRUE;
    }

    void Surface::Cleanup(Renderer* renderer)
    {
        vkDestroySurfaceKHR(renderer->m_Instance, m_Surface, nullptr);
    }

    VkSurfaceFormatKHR Surface::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for(const auto& availableFormat : availableFormats)
        {
            if(availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR Surface::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

        for(const auto& availablePresentMode : availablePresentModes)
        {
            if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
            else if(availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
            {
                bestMode = availablePresentMode;
            }
        }

        return bestMode;
    }

    VkExtent2D Surface::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            VkExtent2D actualExtent = { WIDTH, HEIGHT };

            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

}}}