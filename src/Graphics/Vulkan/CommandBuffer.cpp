#include "Graphics/Vulkan/CommandBuffer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

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

    void CommandBuffer::BindVertexBuffers(
        uint32_t firstBinding,
        const std::vector<VkBuffer>& buffers,
        const std::vector<VkDeviceSize>& offsets) const
    {
        vkCmdBindVertexBuffers(
            m_CurrentCommandBuffer,
            firstBinding,
            buffers.size(),
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