#include "Platform/Graphics/Vulkan/Vulkan.h"

#include "fstream"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"

namespace SteelEngine { namespace Graphics {

#ifdef NDEBUG
    const bool Vulkan::ms_EnableValidationLayers = false;
#else
    const bool Vulkan::ms_EnableValidationLayers = true;
#endif

    const std::vector<const char*> Vulkan::ms_DeviceExtensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    const int Vulkan::MAX_FRAMES_IN_FLIGHT = 2;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        printf("Vulkan validation layer: %s\n", pCallbackData->pMessage);

        return VK_FALSE;
    }

    VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

        if(func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

        if(func != nullptr)
        {
            func(instance, debugMessenger, pAllocator);
        }
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    std::vector<char> readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if(!file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    VkShaderModule createShaderModule(Platform::Vulkan::LogicalDevice* lDevice, const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;

        lDevice->CreateShaderModule(createInfo, &shaderModule);

        return shaderModule;
    }

    VkCommandBuffer beginSingleTimeCommands(Platform::Vulkan::LogicalDevice* lDevice, VkCommandPool commandPool)
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        // vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);
        lDevice->AllocateCommandBuffers(allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void endSingleTimeCommands(Platform::Vulkan::LogicalDevice* lDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkCommandBuffer commandBuffer)
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue);

        // vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
        lDevice->FreeCommandBuffers(commandPool, 1, &commandBuffer);
    }

    bool createBuffer(Platform::Vulkan::PhysicalDevice* pDevice, Platform::Vulkan::LogicalDevice* lDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        lDevice->CreateBuffer(bufferInfo, &buffer);

        VkMemoryRequirements memRequirements;
        lDevice->GetBufferMemoryRequirements(buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = pDevice->FindMemoryType(memRequirements.memoryTypeBits, properties);

        lDevice->AllocateMemory(allocInfo, &bufferMemory);
        lDevice->BindBufferMemory(buffer, bufferMemory);

        return true;
    }

    void copyBuffer(Platform::Vulkan::LogicalDevice* lDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands(lDevice, commandPool);

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        endSingleTimeCommands(lDevice, commandPool, graphicsQueue, commandBuffer);
    }

    void copyBufferToImage(Platform::Vulkan::LogicalDevice* lDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands(lDevice, commandPool);

        VkBufferImageCopy region = {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent =
        {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );

        endSingleTimeCommands(lDevice, commandPool, graphicsQueue, commandBuffer);
    }

    bool transitionImageLayout(Platform::Vulkan::LogicalDevice* lDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands(lDevice, commandPool);

        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } 
        else
        {
            printf("Unsupported layout transition!\n");

            return false;
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        endSingleTimeCommands(lDevice, commandPool, graphicsQueue, commandBuffer);

        return true;
    }

    bool createImage(Platform::Vulkan::PhysicalDevice* pDevice, Platform::Vulkan::LogicalDevice* lDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
    {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if(!lDevice->CreateImage(imageInfo, &image))
        {
            printf("Failed to create image!\n");

            return false;
        }

        VkMemoryRequirements memRequirements;
        lDevice->GetImageMemoryRequirements(image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = pDevice->FindMemoryType(memRequirements.memoryTypeBits, properties);

        if(!lDevice->AllocateMemory(allocInfo, &imageMemory))
        {
            printf("Failed to allocate image memory!\n");

            return false;
        }

        lDevice->BindImageMemory(image, imageMemory);

        return true;
    }

    bool Vulkan::CheckValidationLayerSupport(const std::vector<const char*>& validationLayers)
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for(const char* layerName : validationLayers)
        {
            bool layerFound = false;

            for(const auto& layerProperties : availableLayers)
            {
                if(strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;

                    break;
                }
            }

            if(!layerFound)
            {
                return false;
            }
        }

        return true;
    }

    bool Vulkan::CreateInstance()
    {
        const std::vector<const char*> validationLayers =
        {
            "VK_LAYER_KHRONOS_validation"
        };

        if(ms_EnableValidationLayers)
        {
            if(!CheckValidationLayerSupport(validationLayers))
            {
                printf("Requested layers are not available!\n");

                return false;
            }

            m_Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        VkApplicationInfo applicationInfo = {};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pApplicationName = "TestVulkan";
        applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        applicationInfo.pEngineName = "SteelEngine";
        applicationInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
        applicationInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &applicationInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(m_Extensions.size());
        createInfo.ppEnabledExtensionNames = m_Extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

        if(ms_EnableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);

            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }
        else
        {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = NULL;
        }

        if(vkCreateInstance(&createInfo, NULL, &m_Instance) != VK_SUCCESS)
        {
            return false;
        }
    }

    bool Vulkan::SetupDebugMessenger()
    {
        if(!ms_EnableValidationLayers)
        {
            return true;
        }

        VkDebugUtilsMessengerCreateInfoEXT createInfo;

        populateDebugMessengerCreateInfo(createInfo);

        if(createDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
        {
            printf("Vulkan debug messanger failed!\n");

            return false;
        }

        return true;
    }

    bool Vulkan::SetupGraphicsPipeline()
    {
        m_GraphicsPipeline = new Platform::Vulkan::Pipeline(m_ViewportRenderPass);

        auto attributeDescriptions = Vertex::GetAttributeDescriptions();

        Platform::Vulkan::Pipeline::CreateInfo createInfo;

        createInfo.AddShaderStage(m_LDevice, "D:/Projects/C++/SteelEngine/bin/Resources/Shaders/TestRenderer/vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
        createInfo.AddShaderStage(m_LDevice, "D:/Projects/C++/SteelEngine/bin/Resources/Shaders/TestRenderer/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
        createInfo.AddUniformSampler(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
        createInfo.AddInput(attributeDescriptions.data(), attributeDescriptions.size(), Vertex::GetBindingDescription());
        createInfo.SetInputAssemblyStateCreateInfo();

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = m_ViewportExtent.width;
        viewport.height = m_ViewportExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = m_ViewportExtent;

        createInfo.SetViewport(viewport, scissor);
        createInfo.SetRasterizer();
        createInfo.SetMultisampling();

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional

        createInfo.AddColorBlendAttachment(
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
            VK_FALSE
        );
        createInfo.SetColorBlendState();

        m_GraphicsPipeline->SetupPipeline(m_LDevice, createInfo);

        return true;
    }

    bool Vulkan::SetupCommandPool()
    {
        Platform::Vulkan::PhysicalDevice::QueueFamilyIndices queueFamilyIndices = m_PDevice->FindQueueFamilies(m_Surface);

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.m_GraphicsFamily.value();
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

        if(!m_LDevice->CreateCommandPool(poolInfo, &m_CommandPool))
        {
            printf("Failed to create command pool!\n");

            return false;
        }

        return true;
    }

    bool Vulkan::SetupCommandBuffers()
    {
        m_CommandBuffers.resize(m_SwapChainImageViews.size());

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_CommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

        if(!m_LDevice->AllocateCommandBuffers(allocInfo, m_CommandBuffers.data()))
        {
            printf("Failed to allocate command buffers!\n");

            return false;
        }

        // for(size_t i = 0; i < m_CommandBuffers.size(); i++)
        // {
        //     VkCommandBufferBeginInfo beginInfo = {};
        //     beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        //     if(vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo) != VK_SUCCESS)
        //     {
        //         printf("Failed to begin recording command buffer!\n");

        //         return false;
        //     }

        //     VkRenderPassBeginInfo renderPassInfo = {};
        //     renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        //     renderPassInfo.renderPass = m_RenderPass;
        //     renderPassInfo.framebuffer = m_SwapChainFramebuffers[i];
        //     renderPassInfo.renderArea.offset = { 0, 0 };
        //     renderPassInfo.renderArea.extent = m_SwapChainExtent;

        //     VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        //     renderPassInfo.clearValueCount = 1;
        //     renderPassInfo.pClearValues = &clearColor;

        //     vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        //         vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

        //         VkBuffer vertexBuffers[] = { m_VertexBuffer };
        //         VkDeviceSize offsets[] = { 0 };

        //         vkCmdBindVertexBuffers(m_CommandBuffers[i], 0, 1, vertexBuffers, offsets);
        //         vkCmdBindIndexBuffer(m_CommandBuffers[i], m_IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
        //         vkCmdBindDescriptorSets(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSets[i], 0, nullptr);
        //         vkCmdDrawIndexed(m_CommandBuffers[i], static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);

        //         m_CommandBuffer->Update(m_CommandBuffers[i]);
        //         m_EditorCommands(m_CommandBuffer);

        //     vkCmdEndRenderPass(m_CommandBuffers[i]);

        //     if(vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS)
        //     {
        //         printf("Failed to record command buffer!\n");

        //         return false;
        //     }
        // }

        return true;
    }

    bool Vulkan::SetupSemaphores()
    {
        m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        m_ImagesInFlight.resize(m_SwapChainImageViews.size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            if(!m_LDevice->CreateSemaphore(semaphoreInfo, &m_ImageAvailableSemaphores[i]) ||
                !m_LDevice->CreateSemaphore(semaphoreInfo, &m_RenderFinishedSemaphores[i]) ||
                !m_LDevice->CreateFence(fenceInfo, &m_InFlightFences[i]))
            {
                printf("Failed to create semaphores for a frame!\n");

                return false;
            }
        }

        return true;
    }

    bool Vulkan::SetupVertexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(m_Vertices[0]) * m_Vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        if(!createBuffer(m_PDevice, m_LDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory))
        {
            return false;
        }

        void* data;
        m_LDevice->MapMemory(stagingBufferMemory, bufferSize, &data);
            memcpy(data, m_Vertices.data(), (size_t)bufferSize);
        m_LDevice->UnmapMemory(stagingBufferMemory);

        if(!createBuffer(m_PDevice, m_LDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory))
        {
            return false;
        }

        copyBuffer(m_LDevice, m_CommandPool, m_LDevice->m_GraphicsQueue, stagingBuffer, m_VertexBuffer, bufferSize);

        m_LDevice->DestroyBuffer(stagingBuffer);
        m_LDevice->FreeMemory(stagingBufferMemory);

        return true;
    }

    bool Vulkan::SetupIndexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(m_Indices[0]) * m_Indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(m_PDevice, m_LDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        m_LDevice->MapMemory(stagingBufferMemory, bufferSize, &data);
        memcpy(data, m_Indices.data(), (size_t)bufferSize);
        m_LDevice->UnmapMemory(stagingBufferMemory);

        createBuffer(m_PDevice, m_LDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);
        copyBuffer(m_LDevice, m_CommandPool, m_LDevice->m_GraphicsQueue, stagingBuffer, m_IndexBuffer, bufferSize);

        m_LDevice->DestroyBuffer(stagingBuffer);
        m_LDevice->FreeMemory(stagingBufferMemory);

        return true;
    }

    bool Vulkan::SetupDescriptorSets()
    {
        m_Descriptor = new Platform::Vulkan::DescriptorPool();

        m_Descriptor->SetupDescriptorPool(m_LDevice, m_GraphicsPipeline, 3);
        m_Descriptor->SetupDescriptorSets(m_LDevice, m_GraphicsPipeline, 3, m_SomeTexture);

        return true;
    }

    bool Vulkan::SetupViewport()
    {
        m_ViewportTexture = new Platform::Vulkan::Texture(m_PDevice, m_LDevice, m_CommandPool, m_LDevice->m_GraphicsQueue);

        m_ViewportTexture->Setup(m_ViewportExtent.width, m_ViewportExtent.height, VK_FORMAT_R8G8B8A8_UNORM);

        Platform::Vulkan::RenderPass::CreateInfo viewportRenderPass;

        VkAttachmentDescription attachDesc = {};
        attachDesc.format = VK_FORMAT_R8G8B8A8_UNORM;
		attachDesc.samples = VK_SAMPLE_COUNT_1_BIT;
		attachDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachDesc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        viewportRenderPass.AddAttachment(attachDesc);

        VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

        VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorReference;

        viewportRenderPass.AddSubpass(subpassDescription);

        VkSubpassDependency dep = {};
        dep.srcSubpass = VK_SUBPASS_EXTERNAL;
		dep.dstSubpass = 0;
		dep.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dep.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dep.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        viewportRenderPass.AddDependency(dep);

        if(!m_ViewportRenderPass->SetupRenderPass(viewportRenderPass))
        {
            return false;
        }

        Platform::Vulkan::Framebuffer::CreateInfo createInfo;

        createInfo.AddAttchamnet(m_ViewportTexture);

        m_ViewportFramebuffer->SetupFramebuffer(m_LDevice, createInfo);

        return true;
    }

    Vulkan::Vulkan()
    {
        m_ViewportExtent = { 512, 512 };

        m_PDevice = new Platform::Vulkan::PhysicalDevice();
        m_LDevice = new Platform::Vulkan::LogicalDevice();
        m_Surface = new Platform::Vulkan::Surface();
        m_ViewportRenderPass = new Platform::Vulkan::RenderPass(m_LDevice);
        m_ViewportFramebuffer = new Platform::Vulkan::Framebuffer(m_ViewportRenderPass, m_ViewportExtent);

        m_Instance = VK_NULL_HANDLE;
        m_CurrentFrame = 0;

        m_CommandBuffer = new Platform::Vulkan::CommandBuffer();

        m_EditorDescriptorSets = 0;
    }

    Vulkan::~Vulkan()
    {

    }

    void Vulkan::Test(VkDescriptorImageInfo* info, VkDescriptorSet* desc)
    {
        *info = m_ViewportTexture->GetDescriptorImageInfo();

        m_EditorDescriptorSets = desc;
    }

    bool Vulkan::CreateDescriptorPool(const VkDescriptorPoolCreateInfo& poolInfo, VkDescriptorPool& descriptorPool)
    {
        if(!m_LDevice->CreateDescriptorPool(poolInfo, &descriptorPool))
        {
            printf("Failed to create descriptor pool!\n");

            return false;
        }

        return true;
    }

    bool Vulkan::CreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo& layoutInfo, VkDescriptorSetLayout& descriptorSetLayout)
    {
        if(!m_LDevice->CreateDescriptorSetLayout(layoutInfo, &descriptorSetLayout))
        {
            printf("Failed to create descriptor set layout!\n");

            return false;
        }

        return true;
    }

    bool Vulkan::AllocateDescriptorSet(const VkDescriptorSetAllocateInfo& allocInfo, VkDescriptorSet& descriptorSet)
    {
        if(!m_LDevice->AllocateDescriptorSets(allocInfo, &descriptorSet))
        {
            printf("Failed to allocate descriptor set!\n");

            return false;
        }

        return true;
    }

    bool Vulkan::AllocateDescriptorSets(const VkDescriptorSetAllocateInfo& allocInfo, std::vector<VkDescriptorSet>& descriptorSets)
    {
        if(!m_LDevice->AllocateDescriptorSets(allocInfo, descriptorSets.data()))
        {
            printf("Failed to allocate descriptor sets!\n");

            return false;
        }

        return true;
    }

    bool Vulkan::UpdateDescriptorSets(const std::vector<VkWriteDescriptorSet>& sets)
    {
        m_LDevice->UpdateDescriptorSets(sets.data(), static_cast<uint32_t>(sets.size()));

        return true;
    }

    bool Vulkan::CreatePipelineLayout(const VkPipelineLayoutCreateInfo& pipelineLayoutInfo, VkPipelineLayout& pipelineLayout)
    {
        if(!m_LDevice->CreatePipelineLayout(pipelineLayoutInfo, &pipelineLayout))
        {
            printf("Failed to create pipeline layout!\n");

            return false;
        }

        return false;
    }

    bool Vulkan::CreatePipeline(const VkGraphicsPipelineCreateInfo& pipelineCreateInfo, VkPipeline& pipeline)
    {
        if(!m_LDevice->CreateGraphicsPipeline(pipelineCreateInfo, &pipeline) != VK_SUCCESS)
        {
            printf("Failed to create graphics pipeline!\n");

            return false;
        }

        return true;
    }

    bool Vulkan::Initialize()
    {
        if(!CreateInstance())
        {
            printf("Instance creation failed!\n");

            return false;
        }

        // uint32_t extensionCount = 0;
        // vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        // std::vector<VkExtensionProperties> extensions(extensionCount);
        // vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        // printf("Available vulkan extensions:\n");

        // for(const auto& extension : extensions)
        // {
        //     printf("\t %s\n", extension.extensionName);
        // }

        if(!SetupDebugMessenger())
        {
            printf("Debug messenger creation failed!\n");

            return false;
        }

        if(m_Window->CreateVulkanSurface(m_Instance, (void**)&m_Surface->m_Surface) == SE_FALSE)
        {
            return false;
        }

        if(!m_PDevice->PickPhysicalDevice(m_Instance, m_Surface, ms_DeviceExtensions))
        {
            printf("Picking physical device failed\n");

            return false;
        }

        if(!m_LDevice->SetupLogicalDevice(m_PDevice, m_Surface, ms_DeviceExtensions))
        {
            printf("Setting logical device up failed!\n");

            return false;
        }

        if(!m_Surface->SetupSurface(m_PDevice, m_LDevice))
        {
            printf("Setting surface up failed!\n");

            return false;
        }

        if(!m_Surface->SetupImageViews(m_LDevice, m_SwapChainImageViews))
        {
            printf("Setting image views up failed!\n");

            return false;
        }

        if(!SetupViewport())
        {
            return false;
        }

        if(!SetupGraphicsPipeline())
        {
            printf("Setting graphics pipeline up failed!\n");

            return false;
        }

        if(!SetupCommandPool())
        {
            printf("Setting command pool up failed!\n");

            return false;
        }

        m_SomeTexture = new Platform::Vulkan::Texture(m_PDevice, m_LDevice, m_CommandPool, m_LDevice->m_GraphicsQueue);

        int texWidth;
        int texHeight;
        int texChannels;
        stbi_uc* pixels = stbi_load("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

        if(!pixels)
        {
            printf("Failed to load texture image!\n");

            return false;
        }

        m_SomeTexture->Setup(pixels, texWidth, texHeight);

        stbi_image_free(pixels);

        if(!SetupVertexBuffer())
        {
            printf("Setting vertex buffer up failed!\n");

            return false;
        }

        if(!SetupIndexBuffer())
        {
            printf("Setting index buffer up failed!\n");

            return false;
        }

        if(!SetupDescriptorSets())
        {
            printf("Setting descriptor sets up failed!\n");

            return false;
        }

    // Here we have to bind editor command buffers

        return true;
    }

    bool Vulkan::PostInitialize()
    {
        // m_EditorCommands(m_CommandBuffer);

        if(!SetupCommandBuffers())
        {
            printf("Setting command buffers up failed!\n");

            return false;
        }

        if(!SetupSemaphores())
        {
            printf("Setting semaphores up failed!\n");

            return false;
        }

        return true;
    }

    void Vulkan::SetVulkanExtensions(uint32_t enabledExtensionCount, const char** extensionNames)
    {
        for(uint32_t i = 0; i < enabledExtensionCount; i++)
        {
            m_Extensions.push_back(extensionNames[i]);
        }
    }

    void Vulkan::SetWindow(IWindow* window)
    {
        m_Window = window;
    }

    void Vulkan::ClearColor(float r, float g, float b, float a) const
    {

    }

    void Vulkan::ClearColor(float r, float g, float b, float a)
    {
        m_ClearValues[0] = r;
        m_ClearValues[1] = g;
        m_ClearValues[2] = b;
        m_ClearValues[3] = a;
    }

    void Vulkan::Clear(ClearMask mask) const
    {

    }

    void Vulkan::DrawTriangles(uint32_t count)
    {
        m_LDevice->WaitForFence(m_InFlightFences[m_CurrentFrame]);

        uint32_t imageIndex;

        m_LDevice->AcquireNextImage(m_Surface, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);
        m_CommandBuffer->Update(m_CommandBuffers[imageIndex]);
        m_CommandBuffer->Begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

    // Viewport aka scene render pass
        {
            VkClearValue clearValue;

            memcpy(clearValue.color.float32, m_ClearValues, 4 * sizeof(float));

            m_CommandBuffer->BeginRenderPass(
                m_ViewportRenderPass,
                m_ViewportFramebuffer,
                m_ViewportExtent,
                &clearValue,
                1
            );

            VkViewport viewport = {};
			viewport.width = m_ViewportExtent.width;
			viewport.height = m_ViewportExtent.height;
			viewport.minDepth = 0.f;
			viewport.maxDepth = 1.f;

            m_CommandBuffer->SetViewport(0, { viewport });

            VkRect2D scissor = {};
			scissor.extent.width = m_ViewportExtent.width;
			scissor.extent.height = m_ViewportExtent.height;
			scissor.offset.x = 0;
			scissor.offset.y = 0;

            m_CommandBuffer->SetScissor(0, { scissor });

        // Render scene with models

            m_CommandBuffer->BindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

            VkBuffer vertexBuffers[] = { m_VertexBuffer };
            VkDeviceSize offsets[] = { 0 };

            m_CommandBuffer->BindVertexBuffers(0, 1, vertexBuffers, offsets);
            m_CommandBuffer->BindIndexBuffer(m_IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
            m_CommandBuffer->BindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline, 0, m_Descriptor, imageIndex);
            m_CommandBuffer->DrawIndexed(static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);

            m_CommandBuffer->EndRenderPass();
        }

    // Editor render pass
        {
        // Prepare render data for editor
            m_Update();

            m_EditorCommands(m_CommandBuffer, imageIndex);
        }

        m_CommandBuffer->End();

        if(m_ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
        {
            m_LDevice->WaitForFence(m_ImagesInFlight[imageIndex]);
        }

        m_ImagesInFlight[imageIndex] = m_InFlightFences[m_CurrentFrame];

        m_LDevice->ResetFence(m_InFlightFences[m_CurrentFrame]);

        VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_CommandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if(vkQueueSubmit(m_LDevice->m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
        {
            printf("Failed to submit draw command buffer!\n");
        }

        VkSwapchainKHR swapChains[] = { m_Surface->GetSwapChain() };

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        vkQueuePresentKHR(m_LDevice->m_PresentQueue, &presentInfo);

        m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void Vulkan::Setup(Graphics::IFramebuffer* framebuffer) const
    {

    }

    void Vulkan::Viewport(int x, int y, uint32_t width, uint32_t height) const
    {

    }

    void Vulkan::Viewport(const Graphics::IFramebuffer* framebuffer) const
    {

    }

    void Vulkan::Bind(const Graphics::IFramebuffer* framebuffer) const
    {

    }

    void Vulkan::GenerateFramebuffer(Graphics::IFramebuffer* framebuffer) const
    {

    }

    void Vulkan::FramebufferTexture2D(Graphics::IFramebuffer* framebuffer, Graphics::FramebufferAttachment attachment, bool* hasDepth, std::vector<uint32_t>& res) const
    {

    }

    void Vulkan::GenRenderbuffer(Graphics::IFramebuffer* framebuffer) const
    {

    }

    void Vulkan::BindRenderbuffer(Graphics::IFramebuffer* framebuffer) const
    {

    }

    void Vulkan::RenderbufferStorage(Graphics::IFramebuffer* framebuffer) const
    {

    }

    void Vulkan::FramebufferRenderbuffer(Graphics::IFramebuffer* framebuffer) const
    {

    }

    void Vulkan::DrawBuffers(const void* data, size_t count) const
    {

    }

    void Vulkan::DrawBuffers(const std::vector<uint32_t>& drawBuffers) const
    {

    }

    Graphics::FramebufferStatus Vulkan::CheckFramebufferStatus() const
    {
        return FramebufferStatus::NOT_IMPLEMENTED;
    }

    void Vulkan::BindFramebuffer(uint32_t id) const
    {

    }

    void Vulkan::GenerateTexture(Graphics::ITexture* texture) const
    {
        SteelEngine::Platform::Vulkan::Texture* t = new SteelEngine::Platform::Vulkan::Texture(m_PDevice, m_LDevice, m_CommandPool, m_LDevice->m_GraphicsQueue);

        *texture->GetTextureID_Ptr() = t;
    }

    void Vulkan::Bind(Graphics::ITexture* texture) const
    {

    }

    void Vulkan::TextureParameter(ParameterName name, ParameterValue value) const
    {

    }

    void Vulkan::TextureParameter(ParameterName name, Filter value) const
    {

    }

    void Vulkan::TextureImage2D(ITexture* texture, InternalFormat internalFormat, uint32_t width, uint32_t height, Format format, DataType type, const void* data) const
    {
        SteelEngine::Platform::Vulkan::Texture* t = (SteelEngine::Platform::Vulkan::Texture*)*texture->GetTextureID_Ptr();

        t->Setup(data, width, height);
    }

    void Vulkan::Setup(IShader* shader, const char* vSourceData, const char* fSourceData) const
    {

    }

    void Vulkan::CheckShaderError(void* shader, ShaderFlag flag, bool isProgram, const std::string& errorMessage) const
    {

    }

    void* Vulkan::CreateShader(const char* sourceData, ShaderType type) const
    {
        return 0;
    }

    void Vulkan::Bind(IShader* shader) const
    {

    }

    ShaderUniform Vulkan::GetLocation(IShader* shader, const char* name) const
    {
        return ShaderUniform();
    }

    void Vulkan::Set(IShader* shader, uint32_t index, float v) const
    {

    }

    void Vulkan::Set(IShader* shader, uint32_t index, int v) const
    {

    }

    void Vulkan::Set(IShader* shader, uint32_t index, const glm::vec2& v) const
    {

    }

    void Vulkan::Set(IShader* shader, uint32_t index, const glm::vec3& v) const
    {

    }

    void Vulkan::Set(IShader* shader, uint32_t index, const glm::mat4& v) const
    {

    }

    void Vulkan::Begin(IMesh* mesh, uint32_t vbosCount) const
    {

    }

    void Vulkan::End(IMesh* mesh) const
    {

    }

    void Vulkan::PassVBO(IMesh* mesh, uint32_t vboIndex, const void* data, uint32_t countOfData, size_t sizeOfDataElement, uint8_t countOfElements) const
    {

    }

    void Vulkan::PassIBO(IMesh* mesh, uint32_t vboIndex, const void* data, uint32_t countOfData, size_t sizeOfDataElement) const
    {

    }

    void Vulkan::Draw(IMesh* mesh) const
    {

    }

}}