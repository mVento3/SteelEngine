#pragma once

#include "Platform/Graphics/Vulkan/PhysicalDevice.h"

#include "vector"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    class Surface;

    class LogicalDevice
    {
    private:
        VkDevice m_LogicalDevice;

    public:
        LogicalDevice();
        ~LogicalDevice();

    // TODO: Temp
        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;

        bool SetupLogicalDevice(PhysicalDevice* pDevice, Surface* surface, const std::vector<const char*>& extensions);

        bool CreateSwapChain(const VkSwapchainCreateInfoKHR& createInfo, VkSwapchainKHR* swapChain);
        void GetSwapchainImages(VkSwapchainKHR swapChain, uint32_t* count, VkImage* images);
        bool CreatePipelineLayout(const VkPipelineLayoutCreateInfo& createInfo, VkPipelineLayout* pipelineLayout);
        bool CreateGraphicsPipeline(const VkGraphicsPipelineCreateInfo& createInfo, VkPipeline* graphicsPipeline);
        bool CreateRenderPass(const VkRenderPassCreateInfo& createInfo, VkRenderPass* renderPass);
        bool CreateFramebuffer(const VkFramebufferCreateInfo& createInfo, VkFramebuffer* framebuffer) const;
        bool CreateCommandPool(const VkCommandPoolCreateInfo& createInfo, VkCommandPool* commandPool);
        bool AllocateCommandBuffers(const VkCommandBufferAllocateInfo& allocInfo, VkCommandBuffer* commandBuffers);
        bool FreeCommandBuffers(VkCommandPool commandPool, uint32_t count, VkCommandBuffer* commandBuffers);
        bool CreateSemaphore(const VkSemaphoreCreateInfo& createInfo, VkSemaphore* semaphore);
        bool CreateFence(const VkFenceCreateInfo& createInfo, VkFence* fence);
        bool CreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo& createInfo, VkDescriptorSetLayout* descriptorSetLayout);
        bool CreateDescriptorPool(const VkDescriptorPoolCreateInfo& createInfo, VkDescriptorPool* descriptorPool);
        bool AllocateDescriptorSets(const VkDescriptorSetAllocateInfo& allocInfo, VkDescriptorSet* descriptorSets);
        bool UpdateDescriptorSet(const VkWriteDescriptorSet& info);
        bool UpdateDescriptorSets(const VkWriteDescriptorSet* infos, uint32_t count);
        bool CreateImageView(const VkImageViewCreateInfo& createInfo, VkImageView* imageView);
        bool CreateShaderModule(const VkShaderModuleCreateInfo& createInfo, VkShaderModule* shaderModule);
        bool CreateImage(const VkImageCreateInfo& createInfo, VkImage* image);
        bool CreateSampler(const VkSamplerCreateInfo& createInfo, VkSampler* sampler);

        bool AcquireNextImage(Surface* surface, uint64_t timeout, const VkSemaphore& semaphore, const VkFence& fence, uint32_t* imageIndex);
        bool WaitForFence(const VkFence& fence, uint64_t timeout = UINT64_MAX) const;
        bool ResetFence(const VkFence& fence) const;

        bool CreateBuffer(const VkBufferCreateInfo& createInfo, VkBuffer* buffer);
        bool DestroyBuffer(VkBuffer buffer);
        bool AllocateMemory(const VkMemoryAllocateInfo& allocInfo, VkDeviceMemory* memory);
        bool FreeMemory(VkDeviceMemory memory);
        bool BindBufferMemory(VkBuffer buffer, VkDeviceMemory memory);

        bool GetBufferMemoryRequirements(VkBuffer buffer, VkMemoryRequirements* memoryRequirements);
        bool GetImageMemoryRequirements(VkImage image, VkMemoryRequirements* memoryRequirements);

        bool BindImageMemory(VkImage image, VkDeviceMemory memory);

        bool MapMemory(VkDeviceMemory memory, VkDeviceSize size, void** data, VkMemoryMapFlags flags = 0, VkDeviceSize offset = 0);
        bool UnmapMemory(VkDeviceMemory memory);

        bool FlushMappedMemoryRanges(const VkMappedMemoryRange* mappedMemoryRanges, uint32_t count);

    // TODO: Temp
        inline VkDevice GetLogicalDevice() const { return m_LogicalDevice; }
    };

}}}