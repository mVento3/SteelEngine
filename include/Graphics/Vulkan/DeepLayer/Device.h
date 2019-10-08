#pragma once

#include "Graphics/Vulkan/DeepLayer/LogicalDevice.h"
#include "Graphics/Vulkan/DeepLayer/PhysicalDevice.h"
#include "Graphics/Vulkan/DeepLayer/CommandPool.h"

#include "Graphics/Vulkan/PresentationLayer/Surface.h"
#include "Graphics/Vulkan/PresentationLayer/SwapChain.h"

#include "Graphics/Vulkan/Utils.h"
#include "Graphics/Vulkan/Buffer.h"

#include "vector"
#include "limits"

#include "Graphics/Vulkan/DeepLayer/IDevice.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Device : public IDevice
    {
        friend class Renderer;
    private:
        Result Setup(
            VkInstance instance,
            Surface* surface
        );
        Result CreateSwapChain(
            const Surface* surface,
            SwapChain* swapChain
        );
        Result CreateCommandPool(
            CommandPool* commandPool
        );

    public:
        Device();
        ~Device();

        LogicalDevice*  m_LogicalDevice;
        PhysicalDevice* m_PhysicalDevice;

        Surface*        m_Surface;
        SwapChain*      m_SwapChain;
        CommandPool*    m_CommandPool;

    // Semaphore and fence
        Result CreateSemaphore(
            const VkSemaphoreCreateInfo* semaphoreCreateInfo,
            VkSemaphore& semaphore
        ) const override;
        Result CreateFence(
            const VkFenceCreateInfo* fenceCreateInfo,
            VkFence& fence
        ) const override;
        void DestroySemaphore(
            const VkSemaphore& semaphore
        ) const override;
        void DestroyFence(
            const VkFence& fence
        ) const override;

    // RenderPass
        Result CreateRenderPass(
            const VkRenderPassCreateInfo* renderPassCreateInfo,
            VkRenderPass& renderPass
        ) const override;
        void DestroyRenderPass(
            const VkRenderPass& renderPass
        ) const override;

        void WaitForFences(
            const std::vector<VkFence>& fences,
            VkBool32 waitAll,
            uint64_t timeout
        ) const override;
        void ResetFences(
            const std::vector<VkFence>& fences
        ) const override;

        VkResult AcquireNextImage(
            uint32_t& imageIndex,
            VkSemaphore semaphore,
            VkFence fence,
            uint64_t timeout
        ) const override;

        Result QueueSubmit(
            const VkSubmitInfo* submitInfo,
            const VkFence& fence
        ) const override;
        VkResult QueuePresent(
            const VkPresentInfoKHR* presentInfo
        ) const override;
        void QueueWaitIdle() const override;

        void WaitIdle() const override;

    // Pipeline
        Result CreatePipelineLayout(
            const VkPipelineLayoutCreateInfo* pipelineLayoutCreateInfo,
            VkPipelineLayout& pipelineLayout
        ) const override;
        Result CreatePipelineCache(
            const VkPipelineCacheCreateInfo* pipelineCacheCreateInfo,
            VkPipelineCache& pipelineCache
        ) const override;
        Result CreatePipelines(
            const std::vector<VkGraphicsPipelineCreateInfo>& pipelineInfos,
            VkPipeline& pipeline,
            VkPipelineCache pipelineCache = 0
        ) const override;
        void DestroyPipeline(
            const VkPipeline& pipeline
        ) const override;
        void DestroyPipelineLayout(
            const VkPipelineLayout& pipelineLayout
        ) const override;

        Result CreateDescriptorSetLayout(
            const VkDescriptorSetLayoutCreateInfo* descriptorSetLayoutCreateInfo,
            VkDescriptorSetLayout& descriptorSetLayout
        ) const override;
        Result AllocateDescriptorSets(
            const VkDescriptorSetAllocateInfo* descriptorSetAllocateInfo,
            std::vector<VkDescriptorSet>& descriptorSets
        ) const override;
        Result AllocateDescriptorSet(
            const VkDescriptorSetAllocateInfo* descriptorSetAllocateInfo,
            VkDescriptorSet& descriptorSet
        ) const override;
        void UpdateDescriptorSets(
            const std::vector<VkWriteDescriptorSet>& writeDescriptorSets
        ) const override;

        Result CreateDescriptorPool(
            const VkDescriptorPoolCreateInfo* descriptorPoolCreateInfo,
            VkDescriptorPool& descriptorPool
        ) const override;

        Result CreateFramebuffer(
            const VkFramebufferCreateInfo* framebufferCreateInfo,
            VkFramebuffer& framebuffer
        ) const override;

        Result AllocateCommandBuffers(
            std::vector<VkCommandBuffer>& commandBuffers,
            VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY
        ) const override;
        Result AllocateCommandBuffer(
            VkCommandBuffer& commandBuffer,
            VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY
        ) const override;
        Result FlushCommandBuffer(
            const VkCommandBuffer& commandBuffer,
            const VkQueue& queue = 0
        ) const override;
        Result FreeCommandBuffer(
            const VkCommandBuffer& commandBuffer
        ) const override;

        Result LoadShader(
            const std::string& filename,
            IShader* shader,
            std::vector<VkPipelineShaderStageCreateInfo>& stages
        ) const override;
        void DestroyShader(
            IShader* shader
        ) const override;

        Result CreateImage(
            const VkImageCreateInfo* imageCreateInfo,
            VkImage& image
        ) const override;
        Result BindImageMemory(
            const VkImage& image,
            const VkDeviceMemory& deviceMemory,
            const VkDeviceSize& memoryOffset = 0
        ) const override;

        VkMemoryRequirements GetImageMemoryRequirements(
            const VkImage& image
        ) const override;
        VkMemoryRequirements GetBufferMemoryRequirements(
            const VkBuffer& buffer
        ) const override;
        Result AllocateMemory(
            const VkMemoryAllocateInfo* memoryAllocateInfo,
            VkDeviceMemory& deviceMemory
        ) const override;
        void FreeMemory(
            VkDeviceMemory& memory
        ) const override;
        Result BindBufferMemory(
            const VkBuffer& buffer,
            const VkDeviceMemory& deviceMemory,
            const VkDeviceSize& offset = 0
        ) const override;
        Result MapMemory(
            const VkDeviceMemory& deviceMemory,
            const VkDeviceSize& size,
            void** data,
            const VkMemoryMapFlags& flags = 0,
            const VkDeviceSize& offset = 0
        ) const override;
        Result UnmapMemory(
            VkDeviceMemory& deviceMemory
        ) const override;
        Result FlushMappedMemoryRange(
            const VkMappedMemoryRange* mappedMemoryRange
        ) const override;

        uint32_t FindMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties
        ) const override;

        Result CreateImageView(
            const VkImageViewCreateInfo* imageViewCreateInfo,
            VkImageView& imageView
        ) const override;

        void CreateSampler(
            const VkSamplerCreateInfo* samplerCreateInfo,
            VkSampler& sampler
        ) const override;

        Result CreateBuffer(
            const VkBufferCreateInfo* bufferCreateInfo,
            VkBuffer& buffer
        ) const override;
        void DestroyBuffer(
            VkBuffer& buffer
        ) const override;

        Result Copy(
            const VkDeviceMemory& memory,
            const void* source,
            const size_t& size
        ) const override;
    
    // Framebuffer
        void DestroyFramebuffer(
            const VkFramebuffer& framebuffer
        ) const override;
    };

}}}