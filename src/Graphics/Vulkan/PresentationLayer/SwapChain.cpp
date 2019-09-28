#include "Graphics/Vulkan/PresentationLayer/SwapChain.h"

#include "Graphics/Vulkan/DeepLayer/PhysicalDevice.h"
#include "Graphics/Vulkan/DeepLayer/QueueFamilyIndices.h"
#include "Graphics/Vulkan/DeepLayer/LogicalDevice.h"

#include "Graphics/Vulkan/PresentationLayer/Surface.h"
#include "Graphics/Vulkan/PresentationLayer/SwapChainSupportDetails.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    Result SwapChain::CreateImageViews(const LogicalDevice& logicalDevice)
    {
        m_SwapChainImageViews.resize(m_SwapChainImages.size());

        for(Type::uint32 i = 0; i < m_SwapChainImages.size(); i++)
        {
            VkImageViewCreateInfo createInfo = {};

            createInfo.sType =      VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image =      m_SwapChainImages[i];
            createInfo.viewType =   VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format =     m_SwapChainImageFormat;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask =        VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel =      0;
            createInfo.subresourceRange.levelCount =        1;
            createInfo.subresourceRange.baseArrayLayer =    0;
            createInfo.subresourceRange.layerCount =        1;

            if(vkCreateImageView(logicalDevice.GetLogicalDevice(), &createInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS)
            {
                return SE_FALSE;
            }
        }

        return SE_TRUE;
    }

    SwapChain::SwapChain()
    {

    }

    SwapChain::~SwapChain()
    {

    }

    Result SwapChain::Create(
        const PhysicalDevice& physicalDevice,
        const LogicalDevice& logicalDevice,
        const Surface& surface)
    {
        SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::QuerySwapChainSupport(physicalDevice.m_PhysicalDevice, surface.m_Surface);

        VkSurfaceFormatKHR surfaceFormat = Surface::ChooseSwapSurfaceFormat(swapChainSupport.m_Formats);
        VkPresentModeKHR presentMode = Surface::ChooseSwapPresentMode(swapChainSupport.m_PresentModes);
        VkExtent2D extent = Surface::ChooseSwapExtent(swapChainSupport.m_Capabilities);

        Type::uint32 imageCount = swapChainSupport.m_Capabilities.minImageCount + 1;

        if(swapChainSupport.m_Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.m_Capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.m_Capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};

        createInfo.sType =              VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface =            surface.m_Surface;
        createInfo.minImageCount =      imageCount;
        createInfo.imageFormat =        surfaceFormat.format;
        createInfo.imageColorSpace =    surfaceFormat.colorSpace;
        createInfo.imageExtent =        extent;
        createInfo.imageArrayLayers =   1;
        createInfo.imageUsage =         VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(physicalDevice.m_PhysicalDevice, surface.m_Surface);
        Type::uint32 queueFamilyIndices[] =
            {
                indices.m_GraphicsFamily.value(),
                indices.m_PresentFamily.value()
            };

        if(indices.m_GraphicsFamily != indices.m_PresentFamily)
        {
            createInfo.imageSharingMode =       VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount =  2;
            createInfo.pQueueFamilyIndices =    queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode =       VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount =  0; // Optional
            createInfo.pQueueFamilyIndices =    nullptr; // Optional
        }

        createInfo.preTransform =   swapChainSupport.m_Capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode =    presentMode;
        createInfo.clipped =        VK_TRUE;
        createInfo.oldSwapchain =   VK_NULL_HANDLE;

        if(vkCreateSwapchainKHR(logicalDevice.GetLogicalDevice(), &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        vkGetSwapchainImagesKHR(logicalDevice.GetLogicalDevice(), m_SwapChain, &imageCount, nullptr);

        m_SwapChainImages.resize(imageCount);

        vkGetSwapchainImagesKHR(logicalDevice.GetLogicalDevice(), m_SwapChain, &imageCount, m_SwapChainImages.data());

        m_SwapChainImageFormat = surfaceFormat.format;
        m_SwapChainExtent = extent;

        if(CreateImageViews(logicalDevice) == SE_FALSE)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    void SwapChain::Cleanup(const LogicalDevice& logicalDevice)
    {
        for(auto imageView : m_SwapChainImageViews)
        {
            vkDestroyImageView(logicalDevice.GetLogicalDevice(), imageView, nullptr);
        }

        vkDestroySwapchainKHR(logicalDevice.GetLogicalDevice(), m_SwapChain, nullptr);
    }

}}}