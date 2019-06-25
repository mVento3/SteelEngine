#include "Rendering/Vulkan/LogicalDevice.h"

#include "Rendering/Vulkan/Renderer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    LogicalDevice::LogicalDevice()
    {

    }

    LogicalDevice::~LogicalDevice()
    {

    }

    Result LogicalDevice::Create(const PhysicalDevice& physicalDevice, const Surface& surface)
    {
        QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(physicalDevice, surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<Type::uint32> uniqueQueueFamilies =
            {
                indices.m_GraphicsFamily.value(),
                indices.m_PresentFamily.value()
            };

        float queuePriority = 1.0f;

        for(Type::uint32 queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo = {};

            queueCreateInfo.sType =             VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex =  queueFamily;
            queueCreateInfo.queueCount =        1;
            queueCreateInfo.pQueuePriorities =  &queuePriority;

            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};
        VkDeviceCreateInfo createInfo = {};

        createInfo.sType =                      VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount =       queueCreateInfos.size();
        createInfo.pQueueCreateInfos =          queueCreateInfos.data();
        createInfo.pEnabledFeatures =           &deviceFeatures;
        createInfo.enabledExtensionCount =      Renderer::mc_DeviceExtensions.size();
        createInfo.ppEnabledExtensionNames =    Renderer::mc_DeviceExtensions.data();

        if(Renderer::mc_EnableValidationLayers)
        {
            createInfo.enabledLayerCount =      Renderer::mc_ValidationLayers.size();
            createInfo.ppEnabledLayerNames =    Renderer::mc_ValidationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if(physicalDevice.CreateLogicalDevice(this, createInfo) == SE_FALSE)
        {
            return SE_FALSE;
        }

        vkGetDeviceQueue(m_LogicalDevice, indices.m_GraphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_LogicalDevice, indices.m_PresentFamily.value(), 0, &m_PresentQueue);

        return SE_TRUE;
    }

    Result LogicalDevice::CreateSwapChain(const VkSwapchainCreateInfoKHR& createInfo, SwapChain* swapChain) const
    {
        if(vkCreateSwapchainKHR(m_LogicalDevice, &createInfo, nullptr, &swapChain->m_SwapChain) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

}}}