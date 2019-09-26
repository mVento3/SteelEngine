#pragma once

#include "Graphics/Vulkan/Buffer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class IndexBuffer : public Buffer
    {
    private:

    public:
        IndexBuffer();
        ~IndexBuffer();

        template <typename A>
        Result CreateIndexBuffer(
            const Device* device,
            A* data_,
            Type::uint32 indicesCount
        );
    };

    template <typename A>
    Result IndexBuffer::CreateIndexBuffer(
        const Device* device,
        A* data_,
        Type::uint32 indicesCount)
    {
        m_Count = indicesCount;

        VkDeviceSize size = sizeof(data_[0]) * indicesCount;
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        CreateBuffer(
            device,
            size,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );

        device->Copy(
            stagingBufferMemory,
            data_,
            size
        );

        CreateBuffer(
            device,
            size,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        CopyBuffer(device, stagingBuffer, m_Buffer, size);

        device->DestroyBuffer(
            stagingBuffer
        );
        device->FreeMemory(
            stagingBufferMemory
        );

        return SE_TRUE;
    }

}}}