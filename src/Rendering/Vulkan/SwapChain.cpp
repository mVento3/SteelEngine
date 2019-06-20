#include "Rendering/Vulkan/SwapChain.h"

#include "Rendering/Vulkan/Renderer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    Result SwapChain::CreateImageViews(Renderer* renderer)
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

            if (vkCreateImageView(renderer->m_LogicalDevice->GetLogicalDevice(), &createInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS)
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

    Result SwapChain::Create(Renderer* renderer)
    {
        SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::QuerySwapChainSupport(renderer->m_PhysicalDevice, renderer->m_Surface);

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
        createInfo.surface =            renderer->m_Surface->m_Surface;
        createInfo.minImageCount =      imageCount;
        createInfo.imageFormat =        surfaceFormat.format;
        createInfo.imageColorSpace =    surfaceFormat.colorSpace;
        createInfo.imageExtent =        extent;
        createInfo.imageArrayLayers =   1;
        createInfo.imageUsage =         VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(renderer->m_PhysicalDevice, renderer->m_Surface);
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

        if(renderer->m_LogicalDevice->CreateSwapChain(createInfo, renderer->m_SwapChain) == SE_FALSE)
        {
            return SE_FALSE;
        }

        vkGetSwapchainImagesKHR(renderer->m_LogicalDevice->GetLogicalDevice(), m_SwapChain, &imageCount, nullptr);

        m_SwapChainImages.resize(imageCount);

        vkGetSwapchainImagesKHR(renderer->m_LogicalDevice->GetLogicalDevice(), m_SwapChain, &imageCount, m_SwapChainImages.data());

        m_SwapChainImageFormat = surfaceFormat.format;
        m_SwapChainExtent = extent;

        if(CreateImageViews(renderer) == SE_FALSE)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

}}}