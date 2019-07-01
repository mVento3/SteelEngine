#pragma once

#include "Core/Result.h"
#include "Core/Type.h"

#include "Rendering/Vulkan/Buffer.h"

#include "Vulkan/vulkan.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Renderer;
    class PhysicalDevice;
    class LogicalDevice;
    class CommandPool;

    class VertexBuffer : public Buffer
    {
    private:
        VkMemoryRequirements m_MemoryRequirements;

    public:
        VertexBuffer();
        ~VertexBuffer();

        template <typename A>
        Result CreateVertexBuffer(
            const PhysicalDevice& physicalDevice,
            const LogicalDevice& logicalDevice,
            const CommandPool& commandPool,
            A* data_,
            Type::uint32 verticesCount
        );
    };

    template <typename A>
    Result VertexBuffer::CreateVertexBuffer(
        const PhysicalDevice& physicalDevice,
        const LogicalDevice& logicalDevice,
        const CommandPool& commandPool,
        A* data_,
        Type::uint32 verticesCount)
    {
        m_Count = verticesCount;

        VkDeviceSize size = sizeof(data_[0]) * verticesCount;
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        CreateBuffer(
            physicalDevice,
            logicalDevice,
            size,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );

        void* data;

        vkMapMemory(logicalDevice.GetLogicalDevice(), stagingBufferMemory, 0, size, 0, &data);
        memcpy(data, data_, (size_t)size);
        vkUnmapMemory(logicalDevice.GetLogicalDevice(), stagingBufferMemory);

        CreateBuffer(
            physicalDevice,
            logicalDevice,
            size,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        CopyBuffer(logicalDevice, commandPool, stagingBuffer, m_Buffer, size);

        vkDestroyBuffer(logicalDevice.GetLogicalDevice(), stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice.GetLogicalDevice(), stagingBufferMemory, nullptr);

        return SE_TRUE;
    }

}}}