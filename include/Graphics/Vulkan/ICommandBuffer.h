#pragma once

#include "Vulkan/vulkan.h"

#include "vector"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    struct ICommandBuffer
    {
        virtual void BindDescriptorSets(
            VkPipelineBindPoint pipelineBindPoint,
            VkPipelineLayout pipelineLayout,
            uint32_t firstSet,
            const std::vector<VkDescriptorSet>& descriptorSets,
            const std::vector<uint32_t>& dynamicOffsets
        ) const = 0;
        virtual void BindPipeline(
            VkPipelineBindPoint pipelineBindPoint,
            VkPipeline pipeline
        ) const = 0;
        virtual void SetViewport(
            uint32_t firstViewport,
            const std::vector<VkViewport>& viewports
        ) const = 0;
        virtual void PushConstants(
            VkPipelineLayout layout,
            VkShaderStageFlags stageFlags,
            uint32_t offset,
            uint32_t size,
            const void* values
        ) const = 0;

        virtual void BindVertexBuffers(
            uint32_t firstBinding,
            const std::vector<VkBuffer>& buffers,
            const std::vector<VkDeviceSize>& offsets
        ) const = 0;
        virtual void BindIndexBuffer(
            VkBuffer buffer,
            VkDeviceSize size,
            VkIndexType indexType
        ) const = 0;

        virtual void SetScissor(
            uint32_t fisrtScissor,
            const std::vector<VkRect2D>& scissors
        ) const = 0;

        virtual void DrawIndexed(
            uint32_t indexCount,
            uint32_t instanceCount,
            uint32_t fisrtIndex,
            int32_t vertexOffset,
            uint32_t fisrtInstance
        ) const = 0;

        virtual const VkCommandBuffer& GetCommandBuffer() const = 0;
    };

}}}