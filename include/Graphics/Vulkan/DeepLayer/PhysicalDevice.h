#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

#include "stdexcept"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Surface;
    class LogicalDevice;

    class PhysicalDevice
    {
        friend struct QueueFamilyIndices;
        friend struct SwapChainSupportDetails;
        friend class Renderer;
        friend class CommandPool;

        friend class Device;
        friend class LogicalDevice;
        friend class SwapChain;
    private:
        VkPhysicalDevice m_PhysicalDevice;

        bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

    public:
        PhysicalDevice();
        ~PhysicalDevice();

        Result PickPhysicalDevice(
            VkInstance instance,
            Surface* surface
        );
        Result CreateLogicalDevice(LogicalDevice* logicalDevice, const VkDeviceCreateInfo& createInfo) const;

        Type::uint32 FindMemoryType(
            Type::uint32 typeFilter,
            VkMemoryPropertyFlags properties
        ) const;
    };

}}}