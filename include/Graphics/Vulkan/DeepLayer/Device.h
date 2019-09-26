#pragma once

#include "Graphics/Vulkan/DeepLayer/LogicalDevice.h"
#include "Graphics/Vulkan/DeepLayer/PhysicalDevice.h"
#include "Graphics/Vulkan/DeepLayer/CommandPool.h"

#include "Graphics/Vulkan/PresentationLayer/Surface.h"
#include "Graphics/Vulkan/PresentationLayer/SwapChain.h"

#include "Graphics/Vulkan/Shader.h"
#include "Graphics/Vulkan/Utils.h"
#include "Graphics/Vulkan/Buffer.h"

#include "vector"
#include "limits"

#include "Core/Type.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Device
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

        Result CreateSemaphore(
            const VkSemaphoreCreateInfo* semaphoreCreateInfo,
            VkSemaphore& semaphore
        ) const;
        Result CreateFence(
            const VkFenceCreateInfo* fenceCreateInfo,
            VkFence& fence
        ) const;
        Result CreateRenderPass(
            const VkRenderPassCreateInfo* renderPassCreateInfo,
            VkRenderPass& renderPass
        ) const;

        void WaitForFences(
            const std::vector<VkFence>& fences,
            VkBool32 waitAll,
            uint64_t timeout
        ) const;
        void ResetFences(
            const std::vector<VkFence>& fences
        ) const;

        VkResult AcquireNextImage(
            uint32_t& imageIndex,
            VkSemaphore semaphore,
            VkFence fence,
            uint64_t timeout
        ) const;

        Result QueueSubmit(
            const VkSubmitInfo* submitInfo,
            const VkFence& fence
        ) const;
        VkResult QueuePresent(
            const VkPresentInfoKHR* presentInfo
        ) const;
        void QueueWaitIdle() const;

        void WaitIdle() const;

        Result CreatePipelineLayout(
            const VkPipelineLayoutCreateInfo* pipelineLayoutCreateInfo,
            VkPipelineLayout& pipelineLayout
        ) const;
        Result CreatePipelineCache(
            const VkPipelineCacheCreateInfo* pipelineCacheCreateInfo,
            VkPipelineCache& pipelineCache
        ) const;
        Result CreatePipelines(
            const std::vector<VkGraphicsPipelineCreateInfo>& pipelineInfos,
            VkPipeline& pipeline,
            VkPipelineCache pipelineCache = 0
        ) const;

        Result CreateDescriptorSetLayout(
            const VkDescriptorSetLayoutCreateInfo* descriptorSetLayoutCreateInfo,
            VkDescriptorSetLayout& descriptorSetLayout
        ) const;
        Result AllocateDescriptorSets(
            const VkDescriptorSetAllocateInfo* descriptorSetAllocateInfo,
            std::vector<VkDescriptorSet>& descriptorSets
        ) const;
        Result AllocateDescriptorSet(
            const VkDescriptorSetAllocateInfo* descriptorSetAllocateInfo,
            VkDescriptorSet& descriptorSet
        ) const;
        void UpdateDescriptorSets(
            const std::vector<VkWriteDescriptorSet>& writeDescriptorSets
        ) const;

        Result CreateDescriptorPool(
            const VkDescriptorPoolCreateInfo* descriptorPoolCreateInfo,
            VkDescriptorPool& descriptorPool
        ) const;

        Result CreateFramebuffer(
            const VkFramebufferCreateInfo* framebufferCreateInfo,
            VkFramebuffer& framebuffer
        ) const;

        Result AllocateCommandBuffers(
            std::vector<VkCommandBuffer>& commandBuffers,
            VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY
        ) const;
        Result AllocateCommandBuffer(
            VkCommandBuffer& commandBuffer,
            VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY
        ) const;
        Result FlushCommandBuffer(
            const VkCommandBuffer& commandBuffer,
            const VkQueue& queue = 0
        ) const;
        Result FreeCommandBuffer(
            const VkCommandBuffer& commandBuffer
        ) const;

        Result LoadShader(
            const std::string& filename,
            Shader* shader,
            std::vector<VkPipelineShaderStageCreateInfo>& stages
        ) const;
        void DestroyShader(
            Shader* shader
        ) const;

        Result CreateImage(
            const VkImageCreateInfo* imageCreateInfo,
            VkImage& image
        ) const;
        Result BindImageMemory(
            const VkImage& image,
            const VkDeviceMemory& deviceMemory,
            const VkDeviceSize& memoryOffset = 0
        ) const;

        VkMemoryRequirements GetImageMemoryRequirements(
            const VkImage& image
        ) const;
        VkMemoryRequirements GetBufferMemoryRequirements(
            const VkBuffer& buffer
        ) const;
        Result AllocateMemory(
            const VkMemoryAllocateInfo* memoryAllocateInfo,
            VkDeviceMemory& deviceMemory
        ) const;
        void FreeMemory(
            VkDeviceMemory& memory
        ) const;
        Result BindBufferMemory(
            const VkBuffer& buffer,
            const VkDeviceMemory& deviceMemory,
            const VkDeviceSize& offset = 0
        ) const;
        Result MapMemory(
            const VkDeviceMemory& deviceMemory,
            const VkDeviceSize& size,
            void** data,
            const VkMemoryMapFlags& flags = 0,
            const VkDeviceSize& offset = 0
        ) const;
        Result UnmapMemory(
            VkDeviceMemory& deviceMemory
        ) const;
        Result FlushMappedMemoryRange(
            const VkMappedMemoryRange* mappedMemoryRange
        ) const;

        uint32_t FindMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties
        ) const;

        Result CreateImageView(
            const VkImageViewCreateInfo* imageViewCreateInfo,
            VkImageView& imageView
        ) const;

        void CreateSampler(
            const VkSamplerCreateInfo* samplerCreateInfo,
            VkSampler& sampler
        ) const;

        Result CreateBuffer(
            const VkBufferCreateInfo* bufferCreateInfo,
            VkBuffer& buffer
        ) const;
        void DestroyBuffer(
            VkBuffer& buffer
        ) const;

        template <typename A>
        Result Copy(
            const VkDeviceMemory& memory,
            const A* source
        ) const;
        Result Copy(
            const VkDeviceMemory& memory,
            const void* source,
            const size_t& size
        ) const;
    };

    template <typename A>
    Result Device::Copy(
        const VkDeviceMemory& memory,
        const A* source) const
    {
        return Copy(
            memory,
            source,
            sizeof(A)  
        );
    }

}}}