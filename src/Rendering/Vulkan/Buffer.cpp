#include "Rendering/Vulkan/Buffer.h"

#include "Rendering/Vulkan/LogicalDevice.h"
#include "Rendering/Vulkan/CommandPool.h"

#include "stdexcept"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    Buffer::Buffer()
    {

    }

    Buffer::~Buffer()
    {

    }

    Type::uint32 Buffer::FindMemoryType(const PhysicalDevice& physicalDevice, Type::uint32 typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;

        vkGetPhysicalDeviceMemoryProperties(physicalDevice.m_PhysicalDevice, &memProperties);

        for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        throw std::runtime_error("Failed to find suitable memory type!");
    }

    Result Buffer::CreateBuffer(
        const PhysicalDevice& physicalDevice,
        const LogicalDevice& logicalDevice,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties)
    {
        return CreateBuffer(physicalDevice, logicalDevice, size, usage, properties, m_Buffer, m_BufferMemory);
    }

    Result Buffer::CreateBuffer(
        const PhysicalDevice& physicalDevice,
        const LogicalDevice& logicalDevice,
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

        if(vkCreateBuffer(logicalDevice.GetLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(logicalDevice.GetLogicalDevice(), buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};

        allocInfo.sType =           VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize =  memRequirements.size;
        allocInfo.memoryTypeIndex = Buffer::FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

        if(vkAllocateMemory(logicalDevice.GetLogicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        vkBindBufferMemory(logicalDevice.GetLogicalDevice(), buffer, bufferMemory, 0);

        return SE_TRUE;
    }

    Result Buffer::CopyBuffer(const LogicalDevice& logicalDevice, const CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        VkCommandBufferAllocateInfo allocInfo = {};

        allocInfo.sType =               VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level =               VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool =         commandPool.m_CommandPool;
        allocInfo.commandBufferCount =  1;

        VkCommandBuffer commandBuffer;

        vkAllocateCommandBuffers(logicalDevice.GetLogicalDevice(), &allocInfo, &commandBuffer);

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

        vkQueueSubmit(logicalDevice.m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(logicalDevice.m_GraphicsQueue);

        vkFreeCommandBuffers(logicalDevice.GetLogicalDevice(), commandPool.m_CommandPool, 1, &commandBuffer);

        return SE_TRUE;
    }

    void Buffer::Cleanup(const LogicalDevice& logicalDevice)
    {
        vkDestroyBuffer(logicalDevice.GetLogicalDevice(), m_Buffer, nullptr);
        vkFreeMemory(logicalDevice.GetLogicalDevice(), m_BufferMemory, nullptr);
    }

}}}