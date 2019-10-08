#include "Graphics/Vulkan/DeepLayer/Device.h"

#include "Graphics/Vulkan/PresentationLayer/SwapChainSupportDetails.h"

#include "Graphics/Vulkan/DeepLayer/CommandPool.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    Result Device::Setup(
        VkInstance instance,
        Surface* surface)
    {
        m_Surface = surface;

        if(m_PhysicalDevice->PickPhysicalDevice(instance, surface) == SE_FALSE)
        {
            return SE_FALSE;
        }

        if(m_LogicalDevice->Create(*m_PhysicalDevice, *surface) == SE_FALSE)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    Result Device::CreateSwapChain(
        const Surface* surface,
        SwapChain* swapChain)
    {
        m_SwapChain = swapChain;

        SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::QuerySwapChainSupport(m_PhysicalDevice->m_PhysicalDevice, surface->m_Surface);

        VkSurfaceFormatKHR surfaceFormat = Surface::ChooseSwapSurfaceFormat(swapChainSupport.m_Formats);
        VkPresentModeKHR presentMode = Surface::ChooseSwapPresentMode(swapChainSupport.m_PresentModes);
        VkExtent2D extent = Surface::ChooseSwapExtent(swapChainSupport.m_Capabilities);

        Type::uint32 imageCount = swapChainSupport.m_Capabilities.minImageCount + 1;

        if(swapChainSupport.m_Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.m_Capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.m_Capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};

        createInfo.sType =              VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface =            surface->m_Surface;
        createInfo.minImageCount =      imageCount;
        createInfo.imageFormat =        surfaceFormat.format;
        createInfo.imageColorSpace =    surfaceFormat.colorSpace;
        createInfo.imageExtent =        extent;
        createInfo.imageArrayLayers =   1;
        createInfo.imageUsage =         VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(m_PhysicalDevice->m_PhysicalDevice, surface->m_Surface);
        Type::uint32 queueFamilyIndices[] =
            {
                indices.m_GraphicsFamily.value(),
                indices.m_PresentFamily.value()
            };

        if(indices.m_GraphicsFamily != indices.m_PresentFamily)
        {
            createInfo.imageSharingMode =       VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount =  2;
            createInfo.pQueueFamilyIndices =    queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode =       VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount =  0; // Optional
            createInfo.pQueueFamilyIndices =    nullptr; // Optional
        }

        createInfo.preTransform =   swapChainSupport.m_Capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode =    presentMode;
        createInfo.clipped =        VK_TRUE;
        createInfo.oldSwapchain =   VK_NULL_HANDLE;

        if(vkCreateSwapchainKHR(m_LogicalDevice->m_LogicalDevice, &createInfo, nullptr, &swapChain->m_SwapChain) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        vkGetSwapchainImagesKHR(
            m_LogicalDevice->m_LogicalDevice,
            swapChain->m_SwapChain,
            &imageCount,
            nullptr
        );

        swapChain->m_SwapChainImages.resize(imageCount);

        vkGetSwapchainImagesKHR(
            m_LogicalDevice->m_LogicalDevice,
            swapChain->m_SwapChain,
            &imageCount,
            swapChain->m_SwapChainImages.data()
        );

        swapChain->m_SwapChainImageFormat = surfaceFormat.format;
        swapChain->m_SwapChainExtent = extent;

        swapChain->m_SwapChainImageViews.resize(swapChain->m_SwapChainImages.size());

        for(Type::uint32 i = 0; i < swapChain->m_SwapChainImages.size(); i++)
        {
            VkImageViewCreateInfo createInfo = {};

            createInfo.sType =      VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image =      swapChain->m_SwapChainImages[i];
            createInfo.viewType =   VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format =     swapChain->m_SwapChainImageFormat;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask =        VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel =      0;
            createInfo.subresourceRange.levelCount =        1;
            createInfo.subresourceRange.baseArrayLayer =    0;
            createInfo.subresourceRange.layerCount =        1;

            SE_VK_CHECK(vkCreateImageView(m_LogicalDevice->GetLogicalDevice(), &createInfo, nullptr, &swapChain->m_SwapChainImageViews[i]));
        }

        return SE_TRUE;
    }

    Result Device::CreateCommandPool(
        CommandPool* commandPool)
    {
        m_CommandPool = commandPool;

        QueueFamilyIndices queueFamilyIndices =
            QueueFamilyIndices::FindQueueFamilies(m_PhysicalDevice->m_PhysicalDevice, m_Surface->m_Surface);

        VkCommandPoolCreateInfo commandPoolInfo = {};

        commandPoolInfo.sType =            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolInfo.queueFamilyIndex = queueFamilyIndices.m_GraphicsFamily.value();
        commandPoolInfo.flags =            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        SE_VK_CHECK(
            vkCreateCommandPool(
                m_LogicalDevice->m_LogicalDevice,
                &commandPoolInfo,
                nullptr,
                &commandPool->m_CommandPool
            )
        );

        return SE_TRUE;
    }

    Device::Device()
    {
        m_LogicalDevice = new LogicalDevice();
        m_PhysicalDevice = new PhysicalDevice();
    }

    Device::~Device()
    {

    }

// Semaphore and fence
    Result Device::CreateSemaphore(
        const VkSemaphoreCreateInfo* semaphoreCreateInfo,
        VkSemaphore& semaphore) const
    {
        SE_VK_CHECK(
            vkCreateSemaphore(
                m_LogicalDevice->m_LogicalDevice,
                semaphoreCreateInfo,
                nullptr, 
                &semaphore
            )
        );

        return SE_TRUE;
    }

    Result Device::CreateFence(
        const VkFenceCreateInfo* fenceCreateInfo,
        VkFence& fence) const
    {
        SE_VK_CHECK(vkCreateFence(m_LogicalDevice->m_LogicalDevice, fenceCreateInfo, nullptr, &fence));

        return SE_TRUE;
    }

    void Device::DestroySemaphore(
        const VkSemaphore& semaphore) const
    {
        vkDestroySemaphore(m_LogicalDevice->GetLogicalDevice(), semaphore, 0);
    }

    void Device::DestroyFence(
        const VkFence& fence) const
    {
        vkDestroyFence(m_LogicalDevice->GetLogicalDevice(), fence, 0);
    }

// RenderPass
    Result Device::CreateRenderPass(
        const VkRenderPassCreateInfo* renderPassCreateInfo,
        VkRenderPass& renderPass) const
    {
        SE_VK_CHECK(
            vkCreateRenderPass(
                m_LogicalDevice->m_LogicalDevice,
                renderPassCreateInfo,
                nullptr,
                &renderPass
            )
        );

        return SE_TRUE;
    }

    void Device::DestroyRenderPass(
        const VkRenderPass& renderPass) const
    {
        vkDestroyRenderPass(m_LogicalDevice->GetLogicalDevice(), renderPass, 0);
    }

    void Device::WaitForFences(
        const std::vector<VkFence>& fences,
        VkBool32 waitAll,
        uint64_t timeout) const
    {
        vkWaitForFences(
            m_LogicalDevice->m_LogicalDevice,
            fences.size(),
            fences.data(),
            waitAll,
            timeout
        );
    }

    void Device::ResetFences(
        const std::vector<VkFence>& fences) const
    {
        vkResetFences(
            m_LogicalDevice->m_LogicalDevice,
            fences.size(),
            fences.data()
        );
    }

    VkResult Device::AcquireNextImage(
        uint32_t& imageIndex,
        VkSemaphore semaphore,
        VkFence fence,
        uint64_t timeout) const
    {
        return vkAcquireNextImageKHR(
            m_LogicalDevice->m_LogicalDevice,
            m_SwapChain->m_SwapChain,
            timeout,
            semaphore,
            fence,
            &imageIndex
        );
    }

    Result Device::QueueSubmit(
        const VkSubmitInfo* submitInfo,
        const VkFence& fence) const
    {
        SE_VK_CHECK(
            vkQueueSubmit(
                m_LogicalDevice->m_GraphicsQueue,
                1,
                submitInfo,
                fence
            )
        );

        return SE_TRUE;
    }

    VkResult Device::QueuePresent(
        const VkPresentInfoKHR* presentInfo) const
    {
        return vkQueuePresentKHR(
            m_LogicalDevice->m_PresentQueue,
            presentInfo
        );
    }

    void Device::QueueWaitIdle() const
    {
        vkQueueWaitIdle(
            m_LogicalDevice->m_GraphicsQueue
        );
    }

    void Device::WaitIdle() const
    {
        vkDeviceWaitIdle(
            m_LogicalDevice->m_LogicalDevice
        );
    }

// Pipeline
    Result Device::CreatePipelineLayout(
        const VkPipelineLayoutCreateInfo* pipelineLayoutCreateInfo,
        VkPipelineLayout& pipelineLayout) const
    {
        SE_VK_CHECK(
            vkCreatePipelineLayout(
                m_LogicalDevice->m_LogicalDevice,
                pipelineLayoutCreateInfo,
                nullptr,
                &pipelineLayout
            )
        );

        return SE_TRUE;
    }

    Result Device::CreatePipelineCache(
        const VkPipelineCacheCreateInfo* pipelineCacheCreateInfo,
        VkPipelineCache& pipelineCache) const
    {
        SE_VK_CHECK(
            vkCreatePipelineCache(
                m_LogicalDevice->m_LogicalDevice,
                pipelineCacheCreateInfo,
                nullptr,
                &pipelineCache
            )
        );

        return SE_TRUE;
    }

    Result Device::CreatePipelines(
        const std::vector<VkGraphicsPipelineCreateInfo>& pipelineInfos,
        VkPipeline& pipeline,
        VkPipelineCache pipelineCache) const
    {
        SE_VK_CHECK(vkCreateGraphicsPipelines(
            m_LogicalDevice->m_LogicalDevice,
            pipelineCache,
            pipelineInfos.size(),
            pipelineInfos.data(),
            nullptr,
            &pipeline)
        );

        return SE_TRUE;
    }

    void Device::DestroyPipeline(
        const VkPipeline& pipeline) const
    {
        vkDestroyPipeline(m_LogicalDevice->GetLogicalDevice(), pipeline, 0);
    }

    void Device::DestroyPipelineLayout(
        const VkPipelineLayout& pipelineLayout) const
    {
        vkDestroyPipelineLayout(m_LogicalDevice->GetLogicalDevice(), pipelineLayout, 0);
    }

    Result Device::CreateDescriptorSetLayout(
        const VkDescriptorSetLayoutCreateInfo* descriptorSetLayoutCreateInfo,
        VkDescriptorSetLayout& descriptorSetLayout) const
    {
        SE_VK_CHECK(
            vkCreateDescriptorSetLayout(
                m_LogicalDevice->GetLogicalDevice(),
                descriptorSetLayoutCreateInfo,
                nullptr,
                &descriptorSetLayout
            )
        );

        return SE_TRUE;
    }

    Result Device::AllocateDescriptorSets(
        const VkDescriptorSetAllocateInfo* descriptorSetAllocateInfo,
        std::vector<VkDescriptorSet>& descriptorSets) const
    {
        SE_VK_CHECK(
            vkAllocateDescriptorSets(
                m_LogicalDevice->GetLogicalDevice(),
                descriptorSetAllocateInfo,
                descriptorSets.data()
            )
        );

        return SE_TRUE;
    }

    Result Device::AllocateDescriptorSet(
        const VkDescriptorSetAllocateInfo* descriptorSetAllocateInfo,
        VkDescriptorSet& descriptorSet) const
    {
        SE_VK_CHECK(
            vkAllocateDescriptorSets(
                m_LogicalDevice->GetLogicalDevice(),
                descriptorSetAllocateInfo,
                &descriptorSet
            )
        );

        return SE_TRUE;
    }

    void Device::UpdateDescriptorSets(
        const std::vector<VkWriteDescriptorSet>& writeDescriptorSets) const
    {
        vkUpdateDescriptorSets(
            m_LogicalDevice->GetLogicalDevice(),
            writeDescriptorSets.size(),
            writeDescriptorSets.data(),
            0,
            nullptr
        );
    }

    Result Device::CreateDescriptorPool(
        const VkDescriptorPoolCreateInfo* descriptorPoolCreateInfo,
        VkDescriptorPool& descriptorPool) const
    {
        SE_VK_CHECK(
            vkCreateDescriptorPool(
                m_LogicalDevice->GetLogicalDevice(),
                descriptorPoolCreateInfo,
                nullptr,
                &descriptorPool
            )
        );

        return SE_TRUE;
    }

    Result Device::CreateFramebuffer(
        const VkFramebufferCreateInfo* framebufferCreateInfo,
        VkFramebuffer& framebuffer) const
    {
        SE_VK_CHECK(
            vkCreateFramebuffer(
                m_LogicalDevice->GetLogicalDevice(),
                framebufferCreateInfo,
                nullptr,
                &framebuffer
            )
        );

        return SE_TRUE;
    }

    Result Device::AllocateCommandBuffers(
        std::vector<VkCommandBuffer>& commandBuffers,
        VkCommandBufferLevel level) const
    {
        VkCommandBufferAllocateInfo commandBufferAllocInfo = {};

        commandBufferAllocInfo.sType =               VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocInfo.commandPool =         m_CommandPool->m_CommandPool;
        commandBufferAllocInfo.level =               level;
        commandBufferAllocInfo.commandBufferCount =  commandBuffers.size();

        SE_VK_CHECK(
            vkAllocateCommandBuffers(
                m_LogicalDevice->m_LogicalDevice,
                &commandBufferAllocInfo,
                commandBuffers.data()
            )
        );

        return SE_TRUE;
    }

    Result Device::AllocateCommandBuffer(
        VkCommandBuffer& commandBuffer,
        VkCommandBufferLevel level) const
    {
        VkCommandBufferAllocateInfo commandBufferAllocInfo = {};

        commandBufferAllocInfo.sType =               VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocInfo.commandPool =         m_CommandPool->m_CommandPool;
        commandBufferAllocInfo.level =               level;
        commandBufferAllocInfo.commandBufferCount =  1;

        SE_VK_CHECK(
            vkAllocateCommandBuffers(
                m_LogicalDevice->m_LogicalDevice,
                &commandBufferAllocInfo,
                &commandBuffer
            )
        );

        return SE_TRUE;
    }

    Result Device::FlushCommandBuffer(
        const VkCommandBuffer& commandBuffer,
        const VkQueue& queue) const
    {
        return m_CommandPool->FlushCommandBuffer(
            m_LogicalDevice->m_LogicalDevice,
            commandBuffer,
            (queue == 0 ? m_LogicalDevice->m_GraphicsQueue : queue)
        );
    }

    Result Device::FreeCommandBuffer(
        const VkCommandBuffer& commandBuffer) const
    {
        return m_CommandPool->FreeCommandBuffer(
            m_LogicalDevice->m_LogicalDevice,
            commandBuffer
        );
    }

    Result Device::LoadShader(
        const std::string& filename,
        IShader* shader,
        std::vector<VkPipelineShaderStageCreateInfo>& stages) const
    {
        return shader->LoadShader(
            *m_LogicalDevice,
            filename,
            stages
        );
    }

    void Device::DestroyShader(
        IShader* shader) const
    {
        shader->Destroy(*m_LogicalDevice);
    }

    Result Device::CreateImage(
        const VkImageCreateInfo* imageCreateInfo,
        VkImage& image) const
    {
        SE_VK_CHECK(
            vkCreateImage(
                m_LogicalDevice->m_LogicalDevice,
                imageCreateInfo,
                0,
                &image
            )
        );

        return SE_TRUE;
    }

    Result Device::BindImageMemory(
        const VkImage& image,
        const VkDeviceMemory& deviceMemory,
        const VkDeviceSize& memoryOffset) const
    {
        SE_VK_CHECK(
            vkBindImageMemory(
                m_LogicalDevice->m_LogicalDevice,
                image,
                deviceMemory,
                memoryOffset
            )
        );

        return SE_TRUE;
    }

    VkMemoryRequirements Device::GetImageMemoryRequirements(
        const VkImage& image) const
    {
        VkMemoryRequirements req;

        vkGetImageMemoryRequirements(m_LogicalDevice->m_LogicalDevice, image, &req);

        return req;
    }

    VkMemoryRequirements Device::GetBufferMemoryRequirements(
        const VkBuffer& buffer) const
    {
        VkMemoryRequirements req;

        vkGetBufferMemoryRequirements(m_LogicalDevice->m_LogicalDevice, buffer, &req);

        return req;
    }

    Result Device::AllocateMemory(
        const VkMemoryAllocateInfo* memoryAllocateInfo,
        VkDeviceMemory& deviceMemory) const
    {
        SE_VK_CHECK(
            vkAllocateMemory(
                m_LogicalDevice->m_LogicalDevice,
                memoryAllocateInfo,
                0,
                &deviceMemory
            )
        );

        return SE_TRUE;
    }

    void Device::FreeMemory(
        VkDeviceMemory& memory) const
    {
        vkFreeMemory(
            m_LogicalDevice->m_LogicalDevice,
            memory,
            0
        );
    }

    Result Device::BindBufferMemory(
        const VkBuffer& buffer,
        const VkDeviceMemory& deviceMemory,
        const VkDeviceSize& offset) const
    {
        vkBindBufferMemory(
            m_LogicalDevice->m_LogicalDevice,
            buffer,
            deviceMemory,
            offset
        );

        return SE_TRUE;
    }

    Result Device::MapMemory(
        const VkDeviceMemory& deviceMemory,
        const VkDeviceSize& size,
        void** data,
        const VkMemoryMapFlags& flags,
        const VkDeviceSize& offset) const
    {
        vkMapMemory(
            m_LogicalDevice->m_LogicalDevice,
            deviceMemory,
            offset,
            size,
            flags,
            data
        );

        return SE_TRUE;
    }

    Result Device::UnmapMemory(
        VkDeviceMemory& deviceMemory) const
    {
        vkUnmapMemory(
            m_LogicalDevice->m_LogicalDevice,
            deviceMemory
        );

        return SE_TRUE;
    }

    Result Device::FlushMappedMemoryRange(
        const VkMappedMemoryRange* mappedMemoryRange) const
    {
        vkFlushMappedMemoryRanges(
            m_LogicalDevice->m_LogicalDevice,
            1,
            mappedMemoryRange
        );

        return SE_TRUE;
    }

    uint32_t Device::FindMemoryType(
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties) const
    {
        return m_PhysicalDevice->FindMemoryType(
            typeFilter,
            properties
        );
    }

    Result Device::CreateImageView(
        const VkImageViewCreateInfo* imageViewCreateInfo,
        VkImageView& imageView) const
    {
        SE_VK_CHECK(
            vkCreateImageView(
                m_LogicalDevice->m_LogicalDevice,
                imageViewCreateInfo,
                0,
                &imageView
            )
        );

        return SE_TRUE;
    }

    void Device::CreateSampler(
        const VkSamplerCreateInfo* samplerCreateInfo,
        VkSampler& sampler) const
    {
        vkCreateSampler(
            m_LogicalDevice->m_LogicalDevice,
            samplerCreateInfo,
            0,
            &sampler
        );
    }

    Result Device::CreateBuffer(
        const VkBufferCreateInfo* bufferCreateInfo,
        VkBuffer& buffer) const
    {
        SE_VK_CHECK(
            vkCreateBuffer(
                m_LogicalDevice->m_LogicalDevice,
                bufferCreateInfo,
                0,
                &buffer
            )
        );

        return SE_TRUE;
    }

    void Device::DestroyBuffer(
        VkBuffer& buffer) const
    {
        vkDestroyBuffer(
            m_LogicalDevice->m_LogicalDevice,
            buffer,
            0
        );
    }

    Result Device::Copy(
        const VkDeviceMemory& memory,
        const void* source,
        const size_t& size) const
    {
        void* data;

        vkMapMemory(m_LogicalDevice->GetLogicalDevice(), memory, 0, size, 0, &data);
        memcpy(data, source, size);
        vkUnmapMemory(m_LogicalDevice->GetLogicalDevice(), memory);

        return SE_TRUE;
    }

// Framebuffer
    void Device::DestroyFramebuffer(
        const VkFramebuffer& framebuffer) const
    {
        vkDestroyFramebuffer(m_LogicalDevice->GetLogicalDevice(), framebuffer, 0);
    }

}}}