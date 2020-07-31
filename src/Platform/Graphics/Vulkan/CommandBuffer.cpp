#include "Platform/Graphics/Vulkan/CommandBuffer.h"

#include "Platform/Graphics/Vulkan/RenderPass.h"
#include "Platform/Graphics/Vulkan/Framebuffer.h"
#include "Platform/Graphics/Vulkan/Pipeline.h"
#include "Platform/Graphics/Vulkan/DescriptorPool.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    void CommandBuffer::Update(const VkCommandBuffer& commandBuffer)
    {
        m_CurrentCommandBuffer = commandBuffer;
    }

    CommandBuffer::CommandBuffer()
    {

    }

    CommandBuffer::~CommandBuffer()
    {

    }

    bool CommandBuffer::Begin(VkCommandBufferUsageFlags flags) const
    {
        VkCommandBufferBeginInfo beginInfo = {};

        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = flags;

        if(vkBeginCommandBuffer(m_CurrentCommandBuffer, &beginInfo) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool CommandBuffer::End() const
    {
        if(vkEndCommandBuffer(m_CurrentCommandBuffer) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    void CommandBuffer::BindDescriptorSet(
        VkPipelineBindPoint pipelineBindPoint,
        VkPipelineLayout pipelineLayout,
        uint32_t firstSet,
        const VkDescriptorSet& descriptorSet) const
    {
        vkCmdBindDescriptorSets(
            m_CurrentCommandBuffer,
            pipelineBindPoint,
            pipelineLayout,
            firstSet,
            1,
            &descriptorSet,
            0,
            NULL
        );
    }

    void CommandBuffer::BindDescriptorSets(
        VkPipelineBindPoint pipelineBindPoint,
        VkPipelineLayout pipelineLayout,
        uint32_t firstSet,
        const std::vector<VkDescriptorSet>& descriptorSets,
        const std::vector<uint32_t>& dynamicOffsets) const
    {
        vkCmdBindDescriptorSets(
            m_CurrentCommandBuffer,
            pipelineBindPoint,
            pipelineLayout,
            firstSet,
            descriptorSets.size(),
            descriptorSets.data(),
            dynamicOffsets.size(),
            dynamicOffsets.data()
        );
    }

    void CommandBuffer::BindDescriptorSet(
        VkPipelineBindPoint pipelineBindPoint,
        const Pipeline* pipeline,
        uint32_t firstSet,
        const VkDescriptorSet& descriptorSet) const
    {
        BindDescriptorSet(pipelineBindPoint, pipeline->m_PipelineLayout, firstSet, descriptorSet);
    }

    void CommandBuffer::BindDescriptorSets(
        VkPipelineBindPoint pipelineBindPoint,
        const Pipeline* pipeline,
        uint32_t firstSet,
        const std::vector<VkDescriptorSet>& descriptorSets,
        const std::vector<uint32_t>& dynamicOffsets) const
    {
        BindDescriptorSets(pipelineBindPoint, pipeline->m_PipelineLayout, firstSet, descriptorSets, dynamicOffsets);
    }

    void CommandBuffer::BindDescriptorSet(
        VkPipelineBindPoint pipelineBindPoint,
        const Pipeline* pipeline,
        uint32_t firstSet,
        const DescriptorPool* descriptorPool,
        uint32_t descriptorSetIndex) const
    {
        BindDescriptorSet(pipelineBindPoint, pipeline->m_PipelineLayout, firstSet, descriptorPool->m_DescriptorSets[descriptorSetIndex]);
    }

    void CommandBuffer::BindDescriptorSets(
        VkPipelineBindPoint pipelineBindPoint,
        const Pipeline* pipeline,
        uint32_t firstSet,
        const DescriptorPool* descriptorPool,
        const std::vector<uint32_t>& dynamicOffsets) const
    {
        BindDescriptorSets(pipelineBindPoint, pipeline, firstSet, descriptorPool->m_DescriptorSets, dynamicOffsets);
    }

    void CommandBuffer::BindPipeline(
        VkPipelineBindPoint pipelineBindPoint,
        VkPipeline pipeline) const
    {
        vkCmdBindPipeline(
            m_CurrentCommandBuffer,
            pipelineBindPoint,
            pipeline
        );
    }

    void CommandBuffer::BindPipeline(
        VkPipelineBindPoint pipelineBindPoint,
        const Pipeline* pipeline) const
    {
        BindPipeline(pipelineBindPoint, pipeline->m_Pipeline);
    }

    void CommandBuffer::SetViewport(
        uint32_t firstViewport,
        const std::vector<VkViewport>& viewports) const
    {
        vkCmdSetViewport(
            m_CurrentCommandBuffer,
            firstViewport,
            viewports.size(),
            viewports.data()
        );
    }

    void CommandBuffer::PushConstants(
        VkPipelineLayout layout,
        VkShaderStageFlags stageFlags,
        uint32_t offset,
        uint32_t size,
        const void* values) const
    {
        vkCmdPushConstants(
            m_CurrentCommandBuffer,
            layout,
            stageFlags,
            offset,
            size,
            values
        );
    }

    void CommandBuffer::PushConstants(
        const Pipeline* pipeline,
        VkShaderStageFlags stageFlags,
        uint32_t offset,
        uint32_t size,
        const void* values) const
    {
        PushConstants(
            pipeline->m_PipelineLayout,
            stageFlags,
            offset,
            size,
            values
        );
    }

    void CommandBuffer::BindVertexBuffers(
        uint32_t firstBinding,
        uint32_t buffersCount,
        const VkBuffer* buffers,
        const VkDeviceSize* offsets) const
    {
        vkCmdBindVertexBuffers(
            m_CurrentCommandBuffer,
            firstBinding,
            buffersCount,
            buffers,
            offsets
        );
    }

    void CommandBuffer::BindVertexBuffers(
        uint32_t firstBinding,
        const std::vector<VkBuffer>& buffers,
        const std::vector<VkDeviceSize>& offsets) const
    {
        BindVertexBuffers(
            firstBinding,
            static_cast<uint32_t>(buffers.size()),
            buffers.data(),
            offsets.data()
        );
    }

    void CommandBuffer::BindIndexBuffer(
        VkBuffer buffer,
        VkDeviceSize size,
        VkIndexType indexType) const
    {
        vkCmdBindIndexBuffer(
            m_CurrentCommandBuffer,
            buffer,
            size,
            indexType
        );
    }

    void CommandBuffer::SetScissor(
        uint32_t fisrtScissor,
        const std::vector<VkRect2D>& scissors) const
    {
        vkCmdSetScissor(
            m_CurrentCommandBuffer,
            fisrtScissor,
            scissors.size(),
            scissors.data()
        );
    }

    bool CommandBuffer::BeginRenderPass(
        const RenderPass* renderPass,
        const Framebuffer* framebuffer,
        VkExtent2D renderArea,
        const VkClearValue* clearValues,
        uint32_t clearValueCount) const
    {
        VkRenderPassBeginInfo renderPassInfo = {};

        renderPassInfo.sType =              VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass =         renderPass->m_RenderPass;
        renderPassInfo.framebuffer =        framebuffer->m_Framebuffer;
        renderPassInfo.renderArea.extent =  renderArea;
        renderPassInfo.clearValueCount =    clearValueCount;
        renderPassInfo.pClearValues =       clearValues;

        vkCmdBeginRenderPass(m_CurrentCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        return true;
    }

    bool CommandBuffer::BeginRenderPass(
        const RenderPass* renderPass,
        const Framebuffer* framebuffer,
        VkExtent2D renderArea,
        const std::vector<VkClearValue>& clearValues) const
    {
        VkRenderPassBeginInfo renderPassInfo = {};

        renderPassInfo.sType =              VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass =         renderPass->m_RenderPass;
        renderPassInfo.framebuffer =        framebuffer->m_Framebuffer;
        renderPassInfo.renderArea.extent =  renderArea;
        renderPassInfo.clearValueCount =    static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues =       clearValues.data();

        vkCmdBeginRenderPass(m_CurrentCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        return true;
    }

    bool CommandBuffer::EndRenderPass() const
    {
        vkCmdEndRenderPass(m_CurrentCommandBuffer);

        return true;
    }

    void CommandBuffer::DrawIndexed(
        uint32_t indexCount,
        uint32_t instanceCount,
        uint32_t fisrtIndex,
        int32_t vertexOffset,
        uint32_t fisrtInstance) const
    {
        vkCmdDrawIndexed(
            m_CurrentCommandBuffer,
            indexCount,
            instanceCount,
            fisrtIndex,
            vertexOffset,
            fisrtInstance
        );
    }

}}}