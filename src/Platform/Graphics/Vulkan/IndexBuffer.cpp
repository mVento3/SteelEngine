#include "Platform/Graphics/Vulkan/IndexBuffer.h"

#include "cstring"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    IndexBuffer::IndexBuffer(PhysicalDevice* pDevice, LogicalDevice* lDevice) :
        Buffer(pDevice, lDevice)
    {

    }

    IndexBuffer::~IndexBuffer()
    {

    }

    bool IndexBuffer::CreateIndexBuffer(
        size_t size,
        const void* data,
        uint32_t indicesCount)
    {
        m_Count = indicesCount;

        VkDeviceSize size_ = size * indicesCount;
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
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
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