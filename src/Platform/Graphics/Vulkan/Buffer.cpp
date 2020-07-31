#include "Platform/Graphics/Vulkan/Buffer.h"

#include "Platform/Graphics/Vulkan/Vulkan.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    Buffer::Buffer(PhysicalDevice* pDevice, LogicalDevice* lDevice) :
        m_PDevice(pDevice),
        m_LDevice(lDevice)
    {
        m_Mapped = 0;
        m_Buffer = 0;
        m_BufferMemory = 0;
    }

    Buffer::~Buffer()
    {

    }

    bool Buffer::CreateBuffer(
        PhysicalDevice* pDevice,
        LogicalDevice* lDevice,
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

        lDevice->CreateBuffer(bufferInfo, &buffer);

        VkMemoryRequirements memRequirements;
        lDevice->GetBufferMemoryRequirements(buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};

        allocInfo.sType =           VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize =  memRequirements.size;
        allocInfo.memoryTypeIndex = pDevice->FindMemoryType(
                                        memRequirements.memoryTypeBits,
                                        properties
                                    );

        lDevice->AllocateMemory(allocInfo, &bufferMemory);
        lDevice->BindBufferMemory(buffer, bufferMemory);

        return true;
    }

    bool Buffer::CreateBuffer(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties)
    {
        return CreateBuffer(m_PDevice, m_LDevice, size, usage, properties, m_Buffer, m_BufferMemory);
    }

    bool Buffer::CopyBuffer(
        VkBuffer srcBuffer,
        VkBuffer dstBuffer,
        VkDeviceSize size)
    {
        VkCommandBuffer commandBuffer;

        VkCommandBufferAllocateInfo commandBufferAllocInfo = {};

        commandBufferAllocInfo.sType =               VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocInfo.commandPool =         m_CommandPool;
        commandBufferAllocInfo.level =               VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocInfo.commandBufferCount =  1;

        m_LDevice->AllocateCommandBuffers(commandBufferAllocInfo, &commandBuffer);

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

        vkQueueSubmit(
            m_GraphicsQueue,
            1,
            &submitInfo,
            VK_NULL_HANDLE
        );

        vkQueueWaitIdle(
            m_GraphicsQueue
        );

        m_LDevice->FreeCommandBuffers(m_CommandPool, 1, &commandBuffer);

        return true;
    }

    bool Buffer::CopyBuffer(
        void* srcBuffer,
        VkDeviceSize size)
    {
        m_LDevice->MapMemory(m_BufferMemory, VK_WHOLE_SIZE, &m_Mapped);
        memcpy(m_Mapped, srcBuffer, size);
        m_LDevice->UnmapMemory(m_BufferMemory);

        return true;
    }

    bool Buffer::Map(
        VkDeviceSize size,
        VkDeviceSize offset)
    {
        m_LDevice->MapMemory(m_BufferMemory, size, &m_Mapped, 0, offset);

        return true;
    }

    bool Buffer::Unmap()
    {
        if(m_Mapped)
        {
            m_LDevice->UnmapMemory(m_BufferMemory);
        }

        return true;
    }

    bool Buffer::Flush(
        VkDeviceSize size,
        VkDeviceSize offset)
    {
        VkMappedMemoryRange mappedRange = {};

        mappedRange.sType =     VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory =    m_BufferMemory;
        mappedRange.offset =    offset;
        mappedRange.size =      size;

        m_LDevice->FlushMappedMemoryRanges(&mappedRange, 1);

        return true;
    }

    void Buffer::Cleanup()
    {
        if(m_Buffer)
        {
            m_LDevice->DestroyBuffer(m_Buffer);
        }

        if(m_BufferMemory)
        {
            m_LDevice->FreeMemory(m_BufferMemory);
        }
    }

}}}