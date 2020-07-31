#include "Platform/Graphics/Vulkan/Surface.h"

#include "algorithm"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    VkSurfaceFormatKHR Surface::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for(const auto& availableFormat : availableFormats)
        {
            if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR Surface::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for(const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D Surface::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height)
    {
        if(capabilities.currentExtent.width != UINT32_MAX)
        {
            return capabilities.currentExtent;
        }
        else
        {
            VkExtent2D actualExtent = { width, height };

            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

    VkImageView Surface::CreateImageView(LogicalDevice* lDevice, VkImage image, VkFormat format)
    {
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;

        if(!lDevice->CreateImageView(viewInfo, &imageView))
        {
            return 0;
        }

        return imageView;
    }

    Surface::Surface()
    {

    }

    Surface::~Surface()
    {

    }

    bool Surface::SetupSurface(PhysicalDevice* pDevice, LogicalDevice* lDevice)
    {
        Platform::Vulkan::PhysicalDevice::SwapChainSupportDetails swapChainSupport = pDevice->QuerySwapChainSupport(this);

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.m_Formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.m_PresentModes);
        m_SwapChainExtent = ChooseSwapExtent(swapChainSupport.m_Capabilities, 1920, 1080);

        m_SwapChainImageFormat = surfaceFormat.format;

        uint32_t imageCount = swapChainSupport.m_Capabilities.minImageCount + 1;

        if(swapChainSupport.m_Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.m_Capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.m_Capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_Surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = m_SwapChainExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        Platform::Vulkan::PhysicalDevice::QueueFamilyIndices indices = pDevice->FindQueueFamilies(this);
        uint32_t queueFamilyIndices[] = { indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value() };

        if(indices.m_GraphicsFamily != indices.m_PresentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = swapChainSupport.m_Capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if(!lDevice->CreateSwapChain(createInfo, &m_SwapChain))
        {
            return false;
        }

        lDevice->GetSwapchainImages(m_SwapChain, &imageCount, nullptr);
        m_SwapChainImages.resize(imageCount);
        lDevice->GetSwapchainImages(m_SwapChain, &imageCount, m_SwapChainImages.data());

        return true;
    }

    bool Surface::SetupImageViews(LogicalDevice* lDevice, std::vector<VkImageView>& imageViews)
    {
        imageViews.resize(m_SwapChainImages.size());

        for(size_t i = 0; i < imageViews.size(); i++)
        {
            imageViews[i] = CreateImageView(lDevice, m_SwapChainImages[i], m_SwapChainImageFormat);
        }

        return true;
    }

}}}