#include "Rendering/Vulkan/CommandPool.h"

#include "Rendering/Vulkan/LogicalDevice.h"
#include "Rendering/Vulkan/Framebuffer.h"
#include "Rendering/Vulkan/RenderPass.h"
#include "Rendering/Vulkan/SwapChain.h"
#include "Rendering/Vulkan/QueueFamilyIndices.h"
#include "Rendering/Vulkan/VertexBuffer.h"
#include "Rendering/Vulkan/GraphicsPipeline.h"
#include "Rendering/Vulkan/DescriptorPool.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    CommandPool::CommandPool()
    {

    }

    CommandPool::~CommandPool()
    {

    }

    Result CommandPool::CreateCommandBuffers(
        const LogicalDevice& logicalDevice,
        const Framebuffer& framebuffer,
        const RenderPass& renderpass,
        const SwapChain& swapChain,
        const GraphicsPipeline& graphicsPipeline,
        const DescriptorPool& descriptorPool,
        std::vector<Buffer*> vertexVector,
        Buffer* indexBuffer)
    {
        m_CommandBuffers.resize(framebuffer.m_SwapChainFramebuffers.size());

        VkCommandBufferAllocateInfo allocInfo = {};

        allocInfo.sType =               VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool =         m_CommandPool;
        allocInfo.level =               VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount =  m_CommandBuffers.size();

        if(vkAllocateCommandBuffers(logicalDevice.GetLogicalDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        for (size_t i = 0; i < m_CommandBuffers.size(); i++)
        {
            VkCommandBufferBeginInfo beginInfo = {};

            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

            if(vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo) != VK_SUCCESS)
            {
                return SE_FALSE;
            }

            VkRenderPassBeginInfo renderPassInfo = {};

            renderPassInfo.sType =              VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass =         renderpass.GetRenderPass();
            renderPassInfo.framebuffer =        framebuffer.m_SwapChainFramebuffers[i];
            renderPassInfo.renderArea.offset =  { 0, 0 };
            renderPassInfo.renderArea.extent =  swapChain.m_SwapChainExtent;

            VkClearValue clearColor = { 0.0f, 0.15f, 0.3f, 1.0f };

            renderPassInfo.clearValueCount =    1;
            renderPassInfo.pClearValues =       &clearColor;

            vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

                vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.m_GraphicsPipeline);

                for(Buffer* data : vertexVector)
                {
                    VkBuffer buff = data->GetBuffer();
                    VkDeviceSize offsets[] = { 0 };

                    vkCmdBindVertexBuffers(m_CommandBuffers[i], 0, 1, &buff, offsets);
                    vkCmdBindIndexBuffer(m_CommandBuffers[i], indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
                    vkCmdBindDescriptorSets(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.m_PipelineLayout, 0, 1, &descriptorPool.m_DescriptorSets[i], 0, nullptr);
                    vkCmdDrawIndexed(m_CommandBuffers[i], indexBuffer->GetCount(), 1, 0, 0, 0);
                }

            vkCmdEndRenderPass(m_CommandBuffers[i]);

            if(vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS)
            {
                return SE_FALSE;
            }
        }

        return SE_TRUE;
    }

    void CommandPool::CleanupCommandBuffers(const LogicalDevice& logicalDevice)
    {
        vkFreeCommandBuffers(logicalDevice.GetLogicalDevice(), m_CommandPool, m_CommandBuffers.size(), m_CommandBuffers.data());
    }

    Result CommandPool::Create(
        const PhysicalDevice& physicalDevice,
        const LogicalDevice& logicalDevice,
        const Surface& surface)
    {
        QueueFamilyIndices queueFamilyIndices = QueueFamilyIndices::FindQueueFamilies(physicalDevice, surface);

        VkCommandPoolCreateInfo poolInfo = {};

        poolInfo.sType =            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.m_GraphicsFamily.value();
        poolInfo.flags =            0; // Optional

        if(vkCreateCommandPool(logicalDevice.GetLogicalDevice(), &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
        {
            return SE_FALSE;
        }
        
        return SE_TRUE;
    }

    void CommandPool::Cleanup(const LogicalDevice& logicalDevice)
    {
        CleanupCommandBuffers(logicalDevice);
        vkDestroyCommandPool(logicalDevice.GetLogicalDevice(), m_CommandPool, nullptr);
    }

}}}