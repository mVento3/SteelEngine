#pragma once

#include "vulkan/vulkan.h"

#include "vector"

namespace SteelEngine {

    namespace Graphics {

        class Vulkan;

    }

    namespace Platform { namespace Vulkan {

    class RenderPass;
    class Framebuffer;
    class Pipeline;
    class DescriptorPool;

    class CommandBuffer
    {
        friend class SteelEngine::Graphics::Vulkan;
    private:
        VkCommandBuffer m_CurrentCommandBuffer;

    // It is used for swapping current command buffer
        void Update(const VkCommandBuffer& commandBuffer);

    public:
        CommandBuffer();
        ~CommandBuffer();

    // General

        bool Begin(VkCommandBufferUsageFlags flags) const;
        bool End() const;

    // DescriptorSet

        void BindDescriptorSet(
            VkPipelineBindPoint pipelineBindPoint,
            VkPipelineLayout pipelineLayout,
            uint32_t firstSet,
            const VkDescriptorSet& descriptorSet
        ) const;
        void BindDescriptorSets(
            VkPipelineBindPoint pipelineBindPoint,
            VkPipelineLayout pipelineLayout,
            uint32_t firstSet,
            const std::vector<VkDescriptorSet>& descriptorSets,
            const std::vector<uint32_t>& dynamicOffsets
        ) const;
        void BindDescriptorSet(
            VkPipelineBindPoint pipelineBindPoint,
            const Pipeline* pipeline,
            uint32_t firstSet,
            const VkDescriptorSet& descriptorSet
        ) const;
        void BindDescriptorSets(
            VkPipelineBindPoint pipelineBindPoint,
            const Pipeline* pipeline,
            uint32_t firstSet,
            const std::vector<VkDescriptorSet>& descriptorSets,
            const std::vector<uint32_t>& dynamicOffsets
        ) const;
        void BindDescriptorSet(
            VkPipelineBindPoint pipelineBindPoint,
            const Pipeline* pipeline,
            uint32_t firstSet,
            const DescriptorPool* descriptorPool,
            uint32_t descriptorSetIndex
        ) const;
        void BindDescriptorSets(
            VkPipelineBindPoint pipelineBindPoint,
            const Pipeline* pipeline,
            uint32_t firstSet,
            const DescriptorPool* descriptorPool,
            const std::vector<uint32_t>& dynamicOffsets
        ) const;

    // Pipeline

        void BindPipeline(
            VkPipelineBindPoint pipelineBindPoint,
            VkPipeline pipeline
        ) const;
        void BindPipeline(
            VkPipelineBindPoint pipelineBindPoint,
            const Pipeline* pipeline
        ) const;

        void SetViewport(
            uint32_t firstViewport,
            const std::vector<VkViewport>& viewports
        ) const;

        void PushConstants(
            VkPipelineLayout layout,
            VkShaderStageFlags stageFlags,
            uint32_t offset,
            uint32_t size,
            const void* values
        ) const;
        void PushConstants(
            const Pipeline* pipeline,
            VkShaderStageFlags stageFlags,
            uint32_t offset,
            uint32_t size,
            const void* values
        ) const;

        void BindVertexBuffers(
            uint32_t firstBinding,
            uint32_t buffersCount,
            const VkBuffer* buffers,
            const VkDeviceSize* offsets
        ) const;
        void BindVertexBuffers(
            uint32_t firstBinding,
            const std::vector<VkBuffer>& buffers,
            const std::vector<VkDeviceSize>& offsets
        ) const;

        void BindIndexBuffer(
            VkBuffer buffer,
            VkDeviceSize size,
            VkIndexType indexType
        ) const;

        void SetScissor(
            uint32_t fisrtScissor,
            const std::vector<VkRect2D>& scissors
        ) const;

    // RenderPass

        bool BeginRenderPass(
            const RenderPass* renderPass,
            const Framebuffer* framebuffer,
            VkExtent2D renderArea,
            const VkClearValue* clearValues,
            uint32_t clearValueCount
        ) const;
        bool BeginRenderPass(
            const RenderPass* renderPass,
            const Framebuffer* framebuffer,
            VkExtent2D renderArea,
            const std::vector<VkClearValue>& clearValues
        ) const;

        bool EndRenderPass() const;

        void DrawIndexed(
            uint32_t indexCount,
            uint32_t instanceCount,
            uint32_t fisrtIndex,
            int32_t vertexOffset,
            uint32_t fisrtInstance
        ) const;
    };

}}}