#include "Platform/Graphics/Vulkan/VertexBuffer.h"

#include "cstring"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    VertexBuffer::VertexBuffer(PhysicalDevice* pDevice, LogicalDevice* lDevice) :
        Buffer(pDevice, lDevice)
    {

    }

    VertexBuffer::~VertexBuffer()
    {

    }

    bool VertexBuffer::CreateVertexBuffer(
        size_t size,
        const void* data,
        uint32_t verticesCount)
    {
        m_Count = verticesCount;

        VkDeviceSize size_ = size * verticesCount;
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        CreateBuffer(
            m_PDevice,
            m_LDevice,
            size_,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );

        void* data_;

        // vkMapMemory(m_lDevice, stagingBufferMemory, 0, size_, 0, &data_);
        m_LDevice->MapMemory(stagingBufferMemory, size_, &data_);
        memcpy(data_, data, size_);
        // vkUnmapMemory(m_lDevice, stagingBufferMemory);
        m_LDevice->UnmapMemory(stagingBufferMemory);

        CreateBuffer(
            size_,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        CopyBuffer(
            stagingBuffer,
            m_Buffer,
            size_
        );

        m_LDevice->DestroyBuffer(stagingBuffer);
        m_LDevice->FreeMemory(stagingBufferMemory);

        // vkDestroyBuffer(
        //     m_lDevice,
        //     stagingBuffer,
        //     0
        // );

        // vkFreeMemory(
        //     m_lDevice,
        //     stagingBufferMemory,
        //     0
        // );

        return true;
    }

}}}