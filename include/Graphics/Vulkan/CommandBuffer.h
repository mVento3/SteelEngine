#pragma once

#include "Graphics/Vulkan/ICommandBuffer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class CommandBuffer : public ICommandBuffer
    {
        friend class Renderer;
    private:
        VkCommandBuffer m_CurrentCommandBuffer;

        void Update(const VkCommandBuffer& commandBuffer);

    public:
        CommandBuffer();
        ~CommandBuffer();

        void BindDescriptorSets(
            VkPipelineBindPoint pipelineBindPoint,
            VkPipelineLayout pipelineLayout,
            uint32_t firstSet,
            const std::vector<VkDescriptorSet>& descriptorSets,
            const std::vector<uint32_t>& dynamicOffsets
        ) const override;
        void BindPipeline(
            VkPipelineBindPoint pipelineBindPoint,
            VkPipeline pipeline
        ) const override;
        void SetViewport(
            uint32_t firstViewport,
            const std::vector<VkViewport>& viewports
        ) const override;
        void PushConstants(
            VkPipelineLayout layout,
            VkShaderStageFlags stageFlags,
            uint32_t offset,
            uint32_t size,
            const void* values
        ) const override;

        void BindVertexBuffers(
            uint32_t firstBinding,
            const std::vector<VkBuffer>& buffers,
            const std::vector<VkDeviceSize>& offsets
        ) const override;
        void BindIndexBuffer(
            VkBuffer buffer,
            VkDeviceSize size,
            VkIndexType indexType
        ) const override;

        void SetScissor(
            uint32_t fisrtScissor,
            const std::vector<VkRect2D>& scissors
        ) const override;

        void DrawIndexed(
            uint32_t indexCount,
            uint32_t instanceCount,
            uint32_t fisrtIndex,
            int32_t vertexOffset,
            uint32_t fisrtInstance
        ) const override;

        inline const VkCommandBuffer& GetCommandBuffer() const override { return m_CurrentCommandBuffer; }
    };

}}}