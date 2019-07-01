#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Renderer;
    class LogicalDevice;

    class PhysicalDevice
    {
        friend struct QueueFamilyIndices;
        friend struct SwapChainSupportDetails;
        friend struct Buffer;
    private:
        VkPhysicalDevice m_PhysicalDevice;

        bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

    public:
        PhysicalDevice();
        ~PhysicalDevice();

        Result PickPhysicalDevice(Renderer* renderer);
        Result CreateLogicalDevice(LogicalDevice* logicalDevice, const VkDeviceCreateInfo& createInfo) const;
    };

}}}