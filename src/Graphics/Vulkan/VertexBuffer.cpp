#include "Graphics/Vulkan/VertexBuffer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    VertexBuffer::VertexBuffer()
    {

    }

    VertexBuffer::~VertexBuffer()
    {

    }

    Result VertexBuffer::CreateVertexBuffer(
        const IDevice* device,
        size_t size,
        const void* data,
        Type::uint32 verticesCount)
    {
        m_Count = verticesCount;

        VkDeviceSize size_ = size * verticesCount;
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        CreateBuffer(
            device,
            size_,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );

        device->Copy(
            stagingBufferMemory,
            data,
            size_
        );

        CreateBuffer(
            device,
            size_,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        CopyBuffer(device, stagingBuffer, m_Buffer, size_);

        device->DestroyBuffer(
            stagingBuffer
        );
        device->FreeMemory(
            stagingBufferMemory
        );

        return SE_TRUE;
    }

}}}