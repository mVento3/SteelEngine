#pragma once

#include "Platform/Graphics/Vulkan/LogicalDevice.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    class PhysicalDevice;
    class LogicalDevice;

    class Surface
    {
        friend class PhysicalDevice;
        friend class LogicalDevice;
    private:
        VkExtent2D m_SwapChainExtent;
        VkFormat m_SwapChainImageFormat;
        VkSwapchainKHR m_SwapChain;
        std::vector<VkImage> m_SwapChainImages;

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);
        VkImageView CreateImageView(LogicalDevice* lDevice, VkImage image, VkFormat format);

    public:
        Surface();
        ~Surface();

    // TODO: Temp
        VkSurfaceKHR m_Surface;

        bool SetupSurface(PhysicalDevice* pDevice, LogicalDevice* lDevice);

        bool SetupImageViews(LogicalDevice* lDevice, std::vector<VkImageView>& imageViews);

        inline const VkExtent2D& GetSwapChainExtent() const { return m_SwapChainExtent; }
        inline const VkFormat& GetSwapChainFormat() const { return m_SwapChainImageFormat; }
        inline const VkSwapchainKHR& GetSwapChain() const { return m_SwapChain; }
    };

}}}