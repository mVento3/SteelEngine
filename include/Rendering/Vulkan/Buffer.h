#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class LogicalDevice;
    class PhysicalDevice;
    class CommandPool;

    class Buffer
    {
    public:
        static Type::uint32 FindMemoryType(const PhysicalDevice& physicalDevice, Type::uint32 typeFilter, VkMemoryPropertyFlags properties);

    protected:
        VkBuffer        m_Buffer;
        VkDeviceMemory  m_BufferMemory;
        Type::uint32    m_Count;

        Result CreateBuffer(
            const PhysicalDevice& physicalDevice,
            const LogicalDevice& logicalDevice,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties
        );
        Result CopyBuffer(const LogicalDevice& logicalDevice, const CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    public:
        Buffer();
        ~Buffer();

        static Result CreateBuffer(
            const PhysicalDevice& physicalDevice,
            const LogicalDevice& logicalDevice,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkBuffer& buffer,
            VkDeviceMemory& bufferMemory
        );

        virtual void Cleanup(const LogicalDevice& logicalDevice);

        inline VkBuffer GetBuffer() { return m_Buffer; }
        inline Type::uint32 GetCount() { return m_Count; }
    };

}}}