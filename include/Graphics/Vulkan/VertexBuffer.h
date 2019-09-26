#pragma once

#include "Core/Result.h"
#include "Core/Type.h"

#include "Graphics/Vulkan/Buffer.h"

#include "Vulkan/vulkan.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class VertexBuffer : public Buffer
    {
    private:
        VkMemoryRequirements m_MemoryRequirements;

    public:
        VertexBuffer();
        ~VertexBuffer();

        template <typename A>
        Result CreateVertexBuffer(
            const Device* device,
            A* data_,
            Type::uint32 verticesCount
        );
    };

    template <typename A>
    Result VertexBuffer::CreateVertexBuffer(
        const Device* device,
        A* data_,
        Type::uint32 verticesCount)
    {
        m_Count = verticesCount;

        VkDeviceSize size = sizeof(data_[0]) * verticesCount;
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
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
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