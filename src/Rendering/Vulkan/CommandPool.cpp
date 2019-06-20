#include "Rendering/Vulkan/CommandPool.h"

#include "Rendering/Vulkan/Renderer.h"
#include "Rendering/Vulkan/QueueFamilyIndices.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    Result CommandPool::CreateCommandBuffers()
    {
        m_CommandBuffers.resize(m_Renderer->m_Framebuffer->m_SwapChainFramebuffers.size());

        VkCommandBufferAllocateInfo allocInfo = {};

        allocInfo.sType =               VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool =         m_CommandPool;
        allocInfo.level =               VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount =  m_CommandBuffers.size();

        if(vkAllocateCommandBuffers(m_Renderer->m_LogicalDevice->GetLogicalDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
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
            renderPassInfo.renderPass =         m_Renderer->m_RenderPass->GetRenderPass();
            renderPassInfo.framebuffer =        m_Renderer->m_Framebuffer->m_SwapChainFramebuffers[i];
            renderPassInfo.renderArea.offset =  { 0, 0 };
            renderPassInfo.renderArea.extent =  m_Renderer->m_SwapChain->m_SwapChainExtent;

            VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

            renderPassInfo.clearValueCount =    1;
            renderPassInfo.pClearValues =       &clearColor;

            vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

                vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Renderer->m_GraphicsPipeline->GetPipeline());

                vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(m_CommandBuffers[i]);

            if(vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS)
            {
                return SE_FALSE;
            }
        }

        return SE_TRUE;
    }

    CommandPool::CommandPool(Renderer* renderer) :
        m_Renderer(renderer)
    {

    }

    CommandPool::~CommandPool()
    {

    }

    Result CommandPool::Create()
    {
        QueueFamilyIndices queueFamilyIndices = QueueFamilyIndices::FindQueueFamilies(m_Renderer->m_PhysicalDevice, m_Renderer->m_Surface);

        VkCommandPoolCreateInfo poolInfo = {};

        poolInfo.sType =            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.m_GraphicsFamily.value();
        poolInfo.flags =            0; // Optional

        if(vkCreateCommandPool(m_Renderer->m_LogicalDevice->GetLogicalDevice(), &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        if(CreateCommandBuffers() == SE_FALSE)
        {
            return SE_FALSE;
        }
        
        return SE_TRUE;
    }

}}}