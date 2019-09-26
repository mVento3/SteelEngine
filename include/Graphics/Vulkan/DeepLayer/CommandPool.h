#pragma once

#include "Core/Result.h"

#include "Vulkan/vulkan.h"

#include "vector"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class LogicalDevice;
    class PhysicalDevice;
    class Surface;

    class CommandPool
    {
        friend class Buffer;

        friend class Device;
    private:
        VkCommandPool m_CommandPool;

    public:
        CommandPool();
        ~CommandPool();

        Result Create(
            const PhysicalDevice& physicalDevice,
            const LogicalDevice& logicalDevice,
            const Surface& surface
        );
        Result CreateCommandBuffer(
            const VkDevice& device,
            VkCommandBuffer& commandBuffer,
            VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY
        ) const;
        Result FlushCommandBuffer(
            const VkDevice& device,
            const VkCommandBuffer& commandBuffer,
            const VkQueue& queue
        ) const;
        Result FreeCommandBuffer(
            const VkDevice& device,
            const VkCommandBuffer& commandBuffer
        ) const;
    };

}}}