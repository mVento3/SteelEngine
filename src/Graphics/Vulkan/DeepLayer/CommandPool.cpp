#include "Graphics/Vulkan/DeepLayer/CommandPool.h"

#include "Graphics/Vulkan/DeepLayer/LogicalDevice.h"
#include "Graphics/Vulkan/DeepLayer/PhysicalDevice.h"

#include "Graphics/Vulkan/PresentationLayer/Surface.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    CommandPool::CommandPool()
    {

    }

    CommandPool::~CommandPool()
    {

    }

    Result CommandPool::Create(
        const PhysicalDevice& physicalDevice,
        const LogicalDevice& logicalDevice,
        const Surface& surface)
    {
        QueueFamilyIndices queueFamilyIndices = QueueFamilyIndices::FindQueueFamilies(physicalDevice.m_PhysicalDevice, surface.m_Surface);

        VkCommandPoolCreateInfo commandPoolInfo = {};

        commandPoolInfo.sType =            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolInfo.queueFamilyIndex = queueFamilyIndices.m_GraphicsFamily.value();
        commandPoolInfo.flags =            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if(vkCreateCommandPool(logicalDevice.GetLogicalDevice(), &commandPoolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    Result CommandPool::CreateCommandBuffer(
        const VkDevice& device,
        VkCommandBuffer& commandBuffer,
        VkCommandBufferLevel level) const
    {
        VkCommandBufferAllocateInfo commandBufferAllocInfo = {};

        commandBufferAllocInfo.sType =               VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocInfo.commandPool =         m_CommandPool;
        commandBufferAllocInfo.level =               level;
        commandBufferAllocInfo.commandBufferCount =  1;

        if(vkAllocateCommandBuffers(device, &commandBufferAllocInfo, &commandBuffer) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    Result CommandPool::FlushCommandBuffer(
        const VkDevice& device,
        const VkCommandBuffer& commandBuffer,
        const VkQueue& queue) const
    {
        if(commandBuffer == 0)
        {
            return SE_FALSE;
        }

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo = {};

        submitInfo.sType =              VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers =    &commandBuffer;

        VkFenceCreateInfo fenceInfo = {};

        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = 0;

        VkFence fence;

        vkCreateFence(device, &fenceInfo, nullptr, &fence);
        vkQueueSubmit(queue, 1, &submitInfo, fence);
        vkWaitForFences(device, 1, &fence, VK_TRUE, 100000000000);
        vkDestroyFence(device, fence, nullptr);

        return SE_TRUE;
    }

    Result CommandPool::FreeCommandBuffer(
        const VkDevice& device,
        const VkCommandBuffer& commandBuffer) const
    {
        vkFreeCommandBuffers(device, m_CommandPool, 1, &commandBuffer);

        return SE_TRUE;
    }

}}}