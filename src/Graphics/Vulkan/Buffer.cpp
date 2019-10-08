#include "Graphics/Vulkan/Buffer.h"

#include "Graphics/Vulkan/DeepLayer/IDevice.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    Buffer::Buffer()
    {
        m_Mapped = 0;
        m_Buffer = 0;
        m_BufferMemory = 0;
    }

    Buffer::~Buffer()
    {

    }

    Result Buffer::CreateBuffer(
        const IDevice* device,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties)
    {
        return CreateBuffer(device, size, usage, properties, m_Buffer, m_BufferMemory);
    }

    Result Buffer::CreateBuffer(
        const IDevice* device,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer& buffer,
        VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo = {};

        bufferInfo.sType =          VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size =           size;
        bufferInfo.usage =          usage;
        bufferInfo.sharingMode =    VK_SHARING_MODE_EXCLUSIVE;

        device->CreateBuffer(
            &bufferInfo,
            buffer
        );

        VkMemoryRequirements memRequirements =
            device->GetBufferMemoryRequirements(
                buffer
            );

        VkMemoryAllocateInfo allocInfo = {};

        allocInfo.sType =           VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize =  memRequirements.size;
        allocInfo.memoryTypeIndex = device->FindMemoryType(
                                        memRequirements.memoryTypeBits, properties
                                    );

        device->AllocateMemory(
            &allocInfo,
            bufferMemory
        );

        device->BindBufferMemory(
            buffer,
            bufferMemory
        );

        return SE_TRUE;
    }

    Result Buffer::CopyBuffer(
        const IDevice* device,
        VkBuffer srcBuffer,
        VkBuffer dstBuffer,
        VkDeviceSize size)
    {
        VkCommandBuffer commandBuffer;

        device->AllocateCommandBuffer(
            commandBuffer
        );

        VkCommandBufferBeginInfo beginInfo = {};

        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

            VkBufferCopy copyRegion = {};

            copyRegion.size = size;

            vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo = {};

        submitInfo.sType =              VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers =    &commandBuffer;

        device->QueueSubmit(
            &submitInfo,
            VK_NULL_HANDLE
        );
        device->QueueWaitIdle();
        device->FreeCommandBuffer(
            commandBuffer
        );

        return SE_TRUE;
    }

    Result Buffer::CopyBuffer(
        const IDevice* device,
        void* srcBuffer,
        VkDeviceSize size)
    {
        device->MapMemory(
            m_BufferMemory,
            VK_WHOLE_SIZE,
            &m_Mapped
        );

        memcpy(m_Mapped, srcBuffer, size);

        device->UnmapMemory(
            m_BufferMemory
        );

        return SE_TRUE;
    }

    Result Buffer::Map(
        const IDevice* device,
        VkDeviceSize size,
        VkDeviceSize offset)
    {
        device->MapMemory(
            m_BufferMemory,
            size,
            &m_Mapped,
            0,
            offset
        );

        return SE_TRUE;
    }

    Result Buffer::Unmap(
        const IDevice* device)
    {
        if(m_Mapped)
        {
            device->UnmapMemory(
                m_BufferMemory
            );
        }

        return SE_TRUE;
    }

    Result Buffer::Flush(
        const IDevice* device,
        VkDeviceSize size,
        VkDeviceSize offset)
    {
        VkMappedMemoryRange mappedRange = {};

        mappedRange.sType =     VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory =    m_BufferMemory;
        mappedRange.offset =    offset;
        mappedRange.size =      size;

        device->FlushMappedMemoryRange(
            &mappedRange
        );

        return SE_TRUE;
    }

    void Buffer::Cleanup(
        const IDevice* device)
    {
        if(m_Buffer)
        {
            device->DestroyBuffer(m_Buffer);
        }

        if(m_BufferMemory)
        {
            device->FreeMemory(
                m_BufferMemory
            );
        }
    }

}}}