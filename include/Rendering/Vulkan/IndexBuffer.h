#pragma once

#include "Rendering/Vulkan/Buffer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class IndexBuffer : public Buffer
    {
    private:

    public:
        IndexBuffer();
        ~IndexBuffer();

        template <typename A>
        Result CreateIndexBuffer(
            const PhysicalDevice& physicalDevice,
            const LogicalDevice& logicalDevice,
            const CommandPool& commandPool,
            A* data_,
            Type::uint32 indicesCount
        );
    };

    template <typename A>
    Result IndexBuffer::CreateIndexBuffer(
        const PhysicalDevice& physicalDevice,
        const LogicalDevice& logicalDevice,
        const CommandPool& commandPool,
        A* data_,
        Type::uint32 indicesCount)
    {
        m_Count = indicesCount;

        VkDeviceSize size = sizeof(data_[0]) * indicesCount;
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
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        CopyBuffer(logicalDevice, commandPool, stagingBuffer, m_Buffer, size);

        vkDestroyBuffer(logicalDevice.GetLogicalDevice(), stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice.GetLogicalDevice(), stagingBufferMemory, nullptr);

        return SE_TRUE;
    }

}}}