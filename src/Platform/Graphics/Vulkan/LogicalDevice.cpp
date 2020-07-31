#include "Platform/Graphics/Vulkan/LogicalDevice.h"

#include "Platform/Graphics/Vulkan/Surface.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    LogicalDevice::LogicalDevice()
    {

    }

    LogicalDevice::~LogicalDevice()
    {

    }

    bool LogicalDevice::SetupLogicalDevice(PhysicalDevice* pDevice, Surface* surface, const std::vector<const char*>& extensions)
    {
        Platform::Vulkan::PhysicalDevice::QueueFamilyIndices indices = pDevice->FindQueueFamilies(surface);
        float queuePriority = 1.0f;

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies =
        {
            indices.m_GraphicsFamily.value(),
            indices.m_PresentFamily.value()
        };

        for(uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());;
        createInfo.ppEnabledExtensionNames = extensions.data();

        if(!pDevice->CreateLogicalDevice(createInfo, &m_LogicalDevice))
        {
            printf("Failed to create logical device!\n");

            return false;
        }

        vkGetDeviceQueue(m_LogicalDevice, indices.m_GraphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_LogicalDevice, indices.m_PresentFamily.value(), 0, &m_PresentQueue);
    }

    bool LogicalDevice::CreateSwapChain(const VkSwapchainCreateInfoKHR& createInfo, VkSwapchainKHR* swapChain)
    {
        if(vkCreateSwapchainKHR(m_LogicalDevice, &createInfo, nullptr, swapChain) != VK_SUCCESS)
        {
            printf("Failed to create swap chain!\n");

            return false;
        }

        return true;
    }

    void LogicalDevice::GetSwapchainImages(VkSwapchainKHR swapChain, uint32_t* count, VkImage* images)
    {
        vkGetSwapchainImagesKHR(m_LogicalDevice, swapChain, count, images);
    }

    bool LogicalDevice::CreatePipelineLayout(const VkPipelineLayoutCreateInfo& createInfo, VkPipelineLayout* pipelineLayout)
    {
        if(vkCreatePipelineLayout(m_LogicalDevice, &createInfo, nullptr, pipelineLayout) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::CreateGraphicsPipeline(const VkGraphicsPipelineCreateInfo& createInfo, VkPipeline* graphicsPipeline)
    {
        if(vkCreateGraphicsPipelines(m_LogicalDevice, VK_NULL_HANDLE, 1, &createInfo, nullptr, graphicsPipeline) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::CreateRenderPass(const VkRenderPassCreateInfo& createInfo, VkRenderPass* renderPass)
    {
        if(vkCreateRenderPass(m_LogicalDevice, &createInfo, nullptr, renderPass) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::CreateFramebuffer(const VkFramebufferCreateInfo& createInfo, VkFramebuffer* framebuffer) const
    {
        if(vkCreateFramebuffer(m_LogicalDevice, &createInfo, nullptr, framebuffer) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::CreateCommandPool(const VkCommandPoolCreateInfo& createInfo, VkCommandPool* commandPool)
    {
        if(vkCreateCommandPool(m_LogicalDevice, &createInfo, nullptr, commandPool) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::AllocateCommandBuffers(const VkCommandBufferAllocateInfo& allocInfo, VkCommandBuffer* commandBuffers)
    {
        if(vkAllocateCommandBuffers(m_LogicalDevice, &allocInfo, commandBuffers) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::FreeCommandBuffers(VkCommandPool commandPool, uint32_t count, VkCommandBuffer* commandBuffers)
    {
        vkFreeCommandBuffers(m_LogicalDevice, commandPool, count, commandBuffers);

        return true;
    }

    bool LogicalDevice::CreateSemaphore(const VkSemaphoreCreateInfo& createInfo, VkSemaphore* semaphore)
    {
        if(vkCreateSemaphore(m_LogicalDevice, &createInfo, nullptr, semaphore) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::CreateFence(const VkFenceCreateInfo& createInfo, VkFence* fence)
    {
        if(vkCreateFence(m_LogicalDevice, &createInfo, nullptr, fence) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::CreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo& createInfo, VkDescriptorSetLayout* descriptorSetLayout)
    {
        if(vkCreateDescriptorSetLayout(m_LogicalDevice, &createInfo, nullptr, descriptorSetLayout) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::CreateDescriptorPool(const VkDescriptorPoolCreateInfo& createInfo, VkDescriptorPool* descriptorPool)
    {
        if(vkCreateDescriptorPool(m_LogicalDevice, &createInfo, nullptr, descriptorPool) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::AllocateDescriptorSets(const VkDescriptorSetAllocateInfo& allocInfo, VkDescriptorSet* descriptorSets)
    {
        if(vkAllocateDescriptorSets(m_LogicalDevice, &allocInfo, descriptorSets) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::UpdateDescriptorSet(const VkWriteDescriptorSet& info)
    {
        vkUpdateDescriptorSets(m_LogicalDevice, 1, &info, 0, nullptr);

        return true;
    }

    bool LogicalDevice::UpdateDescriptorSets(const VkWriteDescriptorSet* infos, uint32_t count)
    {
        vkUpdateDescriptorSets(m_LogicalDevice, count, infos, 0, nullptr);

        return true;
    }

    bool LogicalDevice::CreateImageView(const VkImageViewCreateInfo& createInfo, VkImageView* imageView)
    {
        if(vkCreateImageView(m_LogicalDevice, &createInfo, nullptr, imageView) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::CreateShaderModule(const VkShaderModuleCreateInfo& createInfo, VkShaderModule* shaderModule)
    {
        vkCreateShaderModule(m_LogicalDevice, &createInfo, 0, shaderModule);

        return true;
    }

    bool LogicalDevice::CreateImage(const VkImageCreateInfo& createInfo, VkImage* image)
    {
        if(vkCreateImage(m_LogicalDevice, &createInfo, nullptr, image) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::CreateSampler(const VkSamplerCreateInfo& createInfo, VkSampler* sampler)
    {
        if(vkCreateSampler(m_LogicalDevice, &createInfo, nullptr, sampler) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::AcquireNextImage(Surface* surface, uint64_t timeout, const VkSemaphore& semaphore, const VkFence& fence, uint32_t* imageIndex)
    {
        vkAcquireNextImageKHR(m_LogicalDevice, surface->m_SwapChain, timeout, semaphore, fence, imageIndex);

        return true;
    }

    bool LogicalDevice::WaitForFence(const VkFence& fence, uint64_t timeout) const
    {
        vkWaitForFences(m_LogicalDevice, 1, &fence, VK_TRUE, timeout);

        return true;
    }

    bool LogicalDevice::ResetFence(const VkFence& fence) const
    {
        vkResetFences(m_LogicalDevice, 1, &fence);

        return true;
    }

    bool LogicalDevice::CreateBuffer(const VkBufferCreateInfo& createInfo, VkBuffer* buffer)
    {
        if(vkCreateBuffer(m_LogicalDevice, &createInfo, nullptr, buffer) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::DestroyBuffer(VkBuffer buffer)
    {
        vkDestroyBuffer(m_LogicalDevice, buffer, nullptr);

        return true;
    }

    bool LogicalDevice::AllocateMemory(const VkMemoryAllocateInfo& allocInfo, VkDeviceMemory* memory)
    {
        if(vkAllocateMemory(m_LogicalDevice, &allocInfo, nullptr, memory) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool LogicalDevice::FreeMemory(VkDeviceMemory memory)
    {
        vkFreeMemory(m_LogicalDevice, memory, nullptr);

        return true;
    }

    bool LogicalDevice::BindBufferMemory(VkBuffer buffer, VkDeviceMemory memory)
    {
        vkBindBufferMemory(m_LogicalDevice, buffer, memory, 0);

        return true;
    }

    bool LogicalDevice::GetBufferMemoryRequirements(VkBuffer buffer, VkMemoryRequirements* memoryRequirements)
    {
        vkGetBufferMemoryRequirements(m_LogicalDevice, buffer, memoryRequirements);

        return true;
    }

    bool LogicalDevice::GetImageMemoryRequirements(VkImage image, VkMemoryRequirements* memoryRequirements)
    {
        vkGetImageMemoryRequirements(m_LogicalDevice, image, memoryRequirements);

        return true;
    }

    bool LogicalDevice::BindImageMemory(VkImage image, VkDeviceMemory memory)
    {
        vkBindImageMemory(m_LogicalDevice, image, memory, 0);

        return true;
    }

    bool LogicalDevice::MapMemory(VkDeviceMemory memory, VkDeviceSize size, void** data, VkMemoryMapFlags flags, VkDeviceSize offset)
    {
        vkMapMemory(m_LogicalDevice, memory, flags, size, offset, data);

        return true;
    }

    bool LogicalDevice::UnmapMemory(VkDeviceMemory memory)
    {
        vkUnmapMemory(m_LogicalDevice, memory);

        return true;
    }

    bool LogicalDevice::FlushMappedMemoryRanges(const VkMappedMemoryRange* mappedMemoryRanges, uint32_t count)
    {
        vkFlushMappedMemoryRanges(m_LogicalDevice, count, mappedMemoryRanges);

        return true;
    }

}}}