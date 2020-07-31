#include "Platform/Graphics/Vulkan/PhysicalDevice.h"

#include "Platform/Graphics/Vulkan/Surface.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    bool PhysicalDevice::IsDeviceSuitable(const VkPhysicalDevice& pDevice, Surface* surface, const std::vector<const char*>& extensions)
    {
        PhysicalDevice::QueueFamilyIndices indices = FindQueueFamilies(pDevice, surface);

        bool extensionsSupported = CheckDeviceExtensionSupport(pDevice, extensions);
        bool swapChainAdequate = false;

        if(extensionsSupported)
        {
            PhysicalDevice::SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(pDevice, surface);
            swapChainAdequate = !swapChainSupport.m_Formats.empty() && !swapChainSupport.m_PresentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(pDevice, &supportedFeatures);

        return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    PhysicalDevice::QueueFamilyIndices PhysicalDevice::FindQueueFamilies(const VkPhysicalDevice& pDevice, Surface* surface)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, queueFamilies.data());

        int i = 0;

        for(const auto& queueFamily : queueFamilies)
        {
            if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.m_GraphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(pDevice, i, surface->m_Surface, &presentSupport);

            if(presentSupport)
            {
                indices.m_PresentFamily = i;
            }

            if(indices.IsComplete())
            {
                break;
            }

            i++;
        }

        return indices;
    }

    bool PhysicalDevice::CheckDeviceExtensionSupport(const VkPhysicalDevice& pDevice, const std::vector<const char*>& extensions)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());

        for(const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    PhysicalDevice::SwapChainSupportDetails PhysicalDevice::QuerySwapChainSupport(const VkPhysicalDevice& pDevice, Surface* surface)
    {
        PhysicalDevice::SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, surface->m_Surface, &details.m_Capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, surface->m_Surface, &formatCount, nullptr);

        if(formatCount != 0)
        {
            details.m_Formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, surface->m_Surface, &formatCount, details.m_Formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, surface->m_Surface, &presentModeCount, nullptr);

        if(presentModeCount != 0)
        {
            details.m_PresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, surface->m_Surface, &presentModeCount, details.m_PresentModes.data());
        }

        return details;
    }

    PhysicalDevice::PhysicalDevice()
    {

    }

    PhysicalDevice::~PhysicalDevice()
    {

    }

    PhysicalDevice::QueueFamilyIndices PhysicalDevice::FindQueueFamilies(Surface* surface)
    {
        return FindQueueFamilies(m_PhysicalDevice, surface);
    }

    PhysicalDevice::SwapChainSupportDetails PhysicalDevice::QuerySwapChainSupport(Surface* surface)
    {
        return QuerySwapChainSupport(m_PhysicalDevice, surface);
    }

    uint32_t PhysicalDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);

        for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

    // TODO:
        // throw std::runtime_error("Failed to find suitable memory type!");
    }

    void PhysicalDevice::GetPhysicalDeviceFormatProperties(const VkFormat& format, VkFormatProperties* formatProperties)
    {
        vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, formatProperties);
    }

    bool PhysicalDevice::GetSupportedDepthFormat(VkFormat* depthFormat)
    {
        std::vector<VkFormat> depthFormats =
        {
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D24_UNORM_S8_UINT,
            VK_FORMAT_D16_UNORM_S8_UINT,
            VK_FORMAT_D16_UNORM
        };

        for (auto& format : depthFormats)
        {
            VkFormatProperties formatProps;
            GetPhysicalDeviceFormatProperties(format, &formatProps);

            if(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
            {
                *depthFormat = format;

                return true;
            }
        }

        return false;
    }

    bool PhysicalDevice::PickPhysicalDevice(const VkInstance& instance, Surface* surface, const std::vector<const char*>& extensions)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if(deviceCount == 0)
        {
            printf("Failed to find GPUs with Vulkan support!\n");

            return false;
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for(const auto& device : devices)
        {
            if(IsDeviceSuitable(device, surface, extensions))
            {
                m_PhysicalDevice = device;

                break;
            }
        }

        if(m_PhysicalDevice == VK_NULL_HANDLE)
        {
            printf("Failed to find a suitable GPU!\n");

            return false;
        }

        return true;
    }

    bool PhysicalDevice::CreateLogicalDevice(const VkDeviceCreateInfo& createInfo, VkDevice* lDevice)
    {
        if(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, lDevice) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

}}}