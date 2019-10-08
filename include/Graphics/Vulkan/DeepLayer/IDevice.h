#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Type.h"
#include "Core/Result.h"

#include "Graphics/Vulkan/IShader.h"

#include "vector"

#include "RuntimeCompiler/IRuntimeObject.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    struct IDevice : public HotReload::IRuntimeObject
    {
    // Semaphore and femce
        virtual Result CreateSemaphore(
            const VkSemaphoreCreateInfo* semaphoreCreateInfo,
            VkSemaphore& semaphore
        ) const = 0;
        virtual Result CreateFence(
            const VkFenceCreateInfo* fenceCreateInfo,
            VkFence& fence
        ) const = 0;
        virtual void DestroySemaphore(
            const VkSemaphore& semaphore
        ) const = 0;
        virtual void DestroyFence(
            const VkFence& fence
        ) const = 0;

    // RenderPass
        virtual Result CreateRenderPass(
            const VkRenderPassCreateInfo* renderPassCreateInfo,
            VkRenderPass& renderPass
        ) const = 0;
        virtual void DestroyRenderPass(
            const VkRenderPass& renderPass
        ) const = 0;

        virtual void WaitForFences(
            const std::vector<VkFence>& fences,
            VkBool32 waitAll,
            uint64_t timeout
        ) const = 0;
        virtual void ResetFences(
            const std::vector<VkFence>& fences
        ) const = 0;

        virtual VkResult AcquireNextImage(
            uint32_t& imageIndex,
            VkSemaphore semaphore,
            VkFence fence,
            uint64_t timeout
        ) const = 0;

        virtual Result QueueSubmit(
            const VkSubmitInfo* submitInfo,
            const VkFence& fence
        ) const = 0;
        virtual VkResult QueuePresent(
            const VkPresentInfoKHR* presentInfo
        ) const = 0;
        virtual void QueueWaitIdle() const = 0;

        virtual void WaitIdle() const = 0;

    // Pipeline
        virtual Result CreatePipelineLayout(
            const VkPipelineLayoutCreateInfo* pipelineLayoutCreateInfo,
            VkPipelineLayout& pipelineLayout
        ) const = 0;
        virtual Result CreatePipelineCache(
            const VkPipelineCacheCreateInfo* pipelineCacheCreateInfo,
            VkPipelineCache& pipelineCache
        ) const = 0;
        virtual Result CreatePipelines(
            const std::vector<VkGraphicsPipelineCreateInfo>& pipelineInfos,
            VkPipeline& pipeline,
            VkPipelineCache pipelineCache = 0
        ) const = 0;
        virtual void DestroyPipeline(
            const VkPipeline& pipeline
        ) const = 0;
        virtual void DestroyPipelineLayout(
            const VkPipelineLayout& pipelineLayout
        ) const = 0;

        virtual Result CreateDescriptorSetLayout(
            const VkDescriptorSetLayoutCreateInfo* descriptorSetLayoutCreateInfo,
            VkDescriptorSetLayout& descriptorSetLayout
        ) const = 0;
        virtual Result AllocateDescriptorSets(
            const VkDescriptorSetAllocateInfo* descriptorSetAllocateInfo,
            std::vector<VkDescriptorSet>& descriptorSets
        ) const = 0;
        virtual Result AllocateDescriptorSet(
            const VkDescriptorSetAllocateInfo* descriptorSetAllocateInfo,
            VkDescriptorSet& descriptorSet
        ) const = 0;
        virtual void UpdateDescriptorSets(
            const std::vector<VkWriteDescriptorSet>& writeDescriptorSets
        ) const = 0;

        virtual Result CreateDescriptorPool(
            const VkDescriptorPoolCreateInfo* descriptorPoolCreateInfo,
            VkDescriptorPool& descriptorPool
        ) const = 0;

        virtual Result CreateFramebuffer(
            const VkFramebufferCreateInfo* framebufferCreateInfo,
            VkFramebuffer& framebuffer
        ) const = 0;

        virtual Result AllocateCommandBuffers(
            std::vector<VkCommandBuffer>& commandBuffers,
            VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY
        ) const = 0;
        virtual Result AllocateCommandBuffer(
            VkCommandBuffer& commandBuffer,
            VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY
        ) const = 0;
        virtual Result FlushCommandBuffer(
            const VkCommandBuffer& commandBuffer,
            const VkQueue& queue = 0
        ) const = 0;
        virtual Result FreeCommandBuffer(
            const VkCommandBuffer& commandBuffer
        ) const = 0;

        virtual Result LoadShader(
            const std::string& filename,
            IShader* shader,
            std::vector<VkPipelineShaderStageCreateInfo>& stages
        ) const = 0;
        virtual void DestroyShader(
            IShader* shader
        ) const = 0;

        virtual Result CreateImage(
            const VkImageCreateInfo* imageCreateInfo,
            VkImage& image
        ) const = 0;
        virtual Result BindImageMemory(
            const VkImage& image,
            const VkDeviceMemory& deviceMemory,
            const VkDeviceSize& memoryOffset = 0
        ) const = 0;

        virtual VkMemoryRequirements GetImageMemoryRequirements(
            const VkImage& image
        ) const = 0;
        virtual VkMemoryRequirements GetBufferMemoryRequirements(
            const VkBuffer& buffer
        ) const = 0;
        virtual Result AllocateMemory(
            const VkMemoryAllocateInfo* memoryAllocateInfo,
            VkDeviceMemory& deviceMemory
        ) const = 0;
        virtual void FreeMemory(
            VkDeviceMemory& memory
        ) const = 0;
        virtual Result BindBufferMemory(
            const VkBuffer& buffer,
            const VkDeviceMemory& deviceMemory,
            const VkDeviceSize& offset = 0
        ) const = 0;
        virtual Result MapMemory(
            const VkDeviceMemory& deviceMemory,
            const VkDeviceSize& size,
            void** data,
            const VkMemoryMapFlags& flags = 0,
            const VkDeviceSize& offset = 0
        ) const = 0;
        virtual Result UnmapMemory(
            VkDeviceMemory& deviceMemory
        ) const = 0;
        virtual Result FlushMappedMemoryRange(
            const VkMappedMemoryRange* mappedMemoryRange
        ) const = 0;

        virtual uint32_t FindMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties
        ) const = 0;

        virtual Result CreateImageView(
            const VkImageViewCreateInfo* imageViewCreateInfo,
            VkImageView& imageView
        ) const = 0;

        virtual void CreateSampler(
            const VkSamplerCreateInfo* samplerCreateInfo,
            VkSampler& sampler
        ) const = 0;

        virtual Result CreateBuffer(
            const VkBufferCreateInfo* bufferCreateInfo,
            VkBuffer& buffer
        ) const = 0;
        virtual void DestroyBuffer(
            VkBuffer& buffer
        ) const = 0;

        virtual Result Copy(
            const VkDeviceMemory& memory,
            const void* source,
            const size_t& size
        ) const = 0;

    // Framebuffer
        virtual void DestroyFramebuffer(
            const VkFramebuffer& framebuffer
        ) const = 0;
    };

}}}