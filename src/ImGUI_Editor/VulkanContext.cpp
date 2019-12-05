#include "ImGUI_Editor/VulkanContext.h"

#include "RuntimeReflection/Reflection.h"

#include "imgui/imgui_internal.h"

namespace SteelEngine {

    void VulkanContext::PassCommands(Graphics::Vulkan::ICommandBuffer* commandBuffer)
    {
        ImGuiIO& io = ImGui::GetIO();

        commandBuffer->BindDescriptorSets(
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_PipelineLayout,
            0,
            { m_DescriptorSet },
            {  }
        );

        commandBuffer->BindPipeline(
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_Pipeline
        );

        VkViewport viewport = {};

        viewport.width =    io.DisplaySize.x;
        viewport.height =   io.DisplaySize.y;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        commandBuffer->SetViewport(
            0,
            { viewport }
        );

		// UI scale and translate via push constants
		m_PushConstBlock.m_Scale = glm::vec2(2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y);
		m_PushConstBlock.m_Translate = glm::vec2(-1.0f);

        commandBuffer->PushConstants(
            m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstBlock), &m_PushConstBlock
        );

		// Render commands
        ImGuiContext& g = *m_Context;
        ImDrawData* imDrawData = &g.DrawData;
		int32_t vertexOffset = 0;
		int32_t indexOffset = 0;

		if(imDrawData->CmdListsCount > 0)
        {
            commandBuffer->BindVertexBuffers(
                0, { m_VertexBuffer->GetBuffer() }, { 0 }
            );
            commandBuffer->BindIndexBuffer(
                m_IndexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16
            );

			for(int32_t i = 0; i < imDrawData->CmdListsCount; i++)
			{
				const ImDrawList* cmd_list = imDrawData->CmdLists[i];

				for(int32_t j = 0; j < cmd_list->CmdBuffer.Size; j++)
				{
					const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[j];
					VkRect2D scissorRect;

					scissorRect.offset.x = std::max((int32_t)(pcmd->ClipRect.x), 0);
					scissorRect.offset.y = std::max((int32_t)(pcmd->ClipRect.y), 0);
					scissorRect.extent.width = (uint32_t)(pcmd->ClipRect.z - pcmd->ClipRect.x);
					scissorRect.extent.height = (uint32_t)(pcmd->ClipRect.w - pcmd->ClipRect.y);

                    commandBuffer->SetScissor(
                        0, { scissorRect }
                    );
                    commandBuffer->DrawIndexed(
                        pcmd->ElemCount, 1, indexOffset, vertexOffset, 0
                    );

                    indexOffset += pcmd->ElemCount;
				}

				vertexOffset += cmd_list->VtxBuffer.Size;
			}
		}
    }

    void VulkanContext::UpdateBuffers()
    {
        while(!m_BuffersToClean.empty())
        {
            m_Device->DestroyBuffer(m_BuffersToClean.front());

            m_BuffersToClean.pop();
        }

        while(!m_MemoryToClean.empty())
        {
            m_Device->FreeMemory(m_MemoryToClean.front());

            m_MemoryToClean.pop();
        }

        ImGuiContext& g = *m_Context;
        ImDrawData* imDrawData = &g.DrawData;

		VkDeviceSize vertexBufferSize = imDrawData->TotalVtxCount * sizeof(ImDrawVert);
		VkDeviceSize indexBufferSize = imDrawData->TotalIdxCount * sizeof(ImDrawIdx);

        if((vertexBufferSize == 0) || (indexBufferSize == 0))
        {
			return;
		}

        if(m_VertexBuffer->GetBuffer() == 0 || m_VertexCount != imDrawData->TotalVtxCount)
        {
            if(m_VertexBuffer->GetBuffer())
            {
                m_BuffersToClean.push(m_VertexBuffer->GetBuffer());
            }

            if(m_VertexBuffer->GetMemory())
            {
                m_MemoryToClean.push(m_VertexBuffer->GetMemory());
            }

            m_VertexBuffer->Unmap(m_Device);
            m_VertexBuffer->CreateBuffer(
                m_Device,
                vertexBufferSize,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );

            m_VertexCount = imDrawData->TotalVtxCount;

            m_VertexBuffer->Map(m_Device);
        }

        VkDeviceSize indexSize = imDrawData->TotalIdxCount * sizeof(ImDrawIdx);

        if(m_IndexBuffer->GetBuffer() == VK_NULL_HANDLE || m_IndexCount < imDrawData->TotalIdxCount)
        {
            if(m_IndexBuffer->GetBuffer())
            {
                m_BuffersToClean.push(m_IndexBuffer->GetBuffer());
            }

            if(m_IndexBuffer->GetMemory())
            {
                m_MemoryToClean.push(m_IndexBuffer->GetMemory());
            }

			m_IndexBuffer->Unmap(m_Device);
			m_IndexBuffer->CreateBuffer(
                m_Device,
                indexBufferSize,
                VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );

			m_IndexCount = imDrawData->TotalIdxCount;

			m_IndexBuffer->Map(m_Device);
		}

        ImDrawVert* vtxDst = (ImDrawVert*)m_VertexBuffer->GetMapped();
		ImDrawIdx* idxDst = (ImDrawIdx*)m_IndexBuffer->GetMapped();

		for(int n = 0; n < imDrawData->CmdListsCount; n++)
        {
			const ImDrawList* cmd_list = imDrawData->CmdLists[n];

			memcpy(vtxDst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
			memcpy(idxDst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

            vtxDst += cmd_list->VtxBuffer.Size;
			idxDst += cmd_list->IdxBuffer.Size;
		}

		m_VertexBuffer->Flush(m_Device);
		m_IndexBuffer->Flush(m_Device);
    }

    VulkanContext::VulkanContext()
    {

    }

    VulkanContext::~VulkanContext()
    {

    }

    Result VulkanContext::SetImageLayout(
        VkCommandBuffer cmdbuffer,
        VkImage image,
        VkImageAspectFlags aspectMask,
        VkImageLayout oldImageLayout,
        VkImageLayout newImageLayout,
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask)
    {
        VkImageSubresourceRange subresourceRange = {};

        subresourceRange.aspectMask =   aspectMask;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount =   1;
        subresourceRange.layerCount =   1;

        SetImageLayout(
            cmdbuffer,
            image,
            oldImageLayout,
            newImageLayout,
            subresourceRange,
            srcStageMask,
            dstStageMask
        );

        return SE_TRUE;
    }

    Result VulkanContext::SetImageLayout(
        VkCommandBuffer cmdbuffer,
        VkImage image,
        VkImageLayout oldImageLayout,
        VkImageLayout newImageLayout,
        VkImageSubresourceRange subresourceRange,
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask)
    {
        VkImageMemoryBarrier imageMemoryBarrier = {};

        imageMemoryBarrier.sType =              VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.oldLayout =          oldImageLayout;
        imageMemoryBarrier.newLayout =          newImageLayout;
        imageMemoryBarrier.image =              image;
        imageMemoryBarrier.subresourceRange =   subresourceRange;

        switch(oldImageLayout)
        {
        case VK_IMAGE_LAYOUT_UNDEFINED:
            // Image layout is undefined (or does not matter)
            // Only valid as initial layout
            // No flags required, listed only for completeness
            imageMemoryBarrier.srcAccessMask = 0;
            break;
        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            // Image is preinitialized
            // Only valid as initial layout for linear images, preserves memory contents
            // Make sure host writes have been finished
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            // Image is a color attachment
            // Make sure any writes to the color buffer have been finished
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            // Image is a depth/stencil attachment
            // Make sure any writes to the depth/stencil buffer have been finished
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            // Image is a transfer source 
            // Make sure any reads from the image have been finished
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            // Image is a transfer destination
            // Make sure any writes to the image have been finished
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            // Image is read by a shader
            // Make sure any shader reads from the image have been finished
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;
        default:
            // Other source layouts aren't handled (yet)
            break;
        }

        // Target layouts (new)
        // Destination access mask controls the dependency for the new image layout
        switch(newImageLayout)
        {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            // Image will be used as a transfer destination
            // Make sure any writes to the image have been finished
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            // Image will be used as a transfer source
            // Make sure any reads from the image have been finished
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            break;
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            // Image will be used as a color attachment
            // Make sure any writes to the color buffer have been finished
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            // Image layout will be used as a depth/stencil attachment
            // Make sure any writes to depth/stencil buffer have been finished
            imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            // Image will be read in a shader (sampler, input attachment)
            // Make sure any writes to the image have been finished
            if(imageMemoryBarrier.srcAccessMask == 0)
            {
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
            }

            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;
        default:
            // Other source layouts aren't handled (yet)
            break;
        }

        // Put barrier inside setup command buffer
        vkCmdPipelineBarrier(
            cmdbuffer,
            srcStageMask,
            dstStageMask,
            0,
            0, nullptr,
            0, nullptr,
            1, &imageMemoryBarrier);

        return SE_TRUE;
    }

    VkVertexInputAttributeDescription VulkanContext::VertexInputAttributeDescription(
        uint32_t binding,
        uint32_t location,
        VkFormat format,
        uint32_t offset)
    {
        VkVertexInputAttributeDescription vInputAttribDescription = {};

        vInputAttribDescription.location =  location;
        vInputAttribDescription.binding =   binding;
        vInputAttribDescription.format =    format;
        vInputAttribDescription.offset =    offset;

        return vInputAttribDescription;
    }

    void VulkanContext::Init(IWindow* window, Graphics::IRenderer* renderer)
    {
        m_Window = window;
        m_Renderer = (Graphics::IRendererAPI<Graphics::IRenderer::API::VULKAN_API>*)renderer;

        m_ImGUI_Shader =
            (Graphics::Vulkan::IShader*)Reflection::CreateInstance("SteelEngine::Graphics::Vulkan::Shader");

        Graphics::IRendererAPI<Graphics::IRenderer::API::VULKAN_API>* rendererAPI =
            (Graphics::IRendererAPI<Graphics::IRenderer::API::VULKAN_API>*)renderer;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForVulkan((SDL_Window*)window->GetWindow());

        m_Context = ImGui::GetCurrentContext();

        unsigned char* fontData;
		int texWidth;
        int texHeight;

		io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);

		VkDeviceSize uploadSize = texWidth * texHeight * 4 * sizeof(char);

        VkImageCreateInfo imageInfo = {};

        imageInfo.sType =           VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType =       VK_IMAGE_TYPE_2D;
		imageInfo.format =          VK_FORMAT_R8G8B8A8_UNORM;
		imageInfo.extent.width =    texWidth;
		imageInfo.extent.height =   texHeight;
		imageInfo.extent.depth =    1;
		imageInfo.mipLevels =       1;
		imageInfo.arrayLayers =     1;
		imageInfo.samples =         VK_SAMPLE_COUNT_1_BIT;
		imageInfo.tiling =          VK_IMAGE_TILING_OPTIMAL;
		imageInfo.usage =           VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		imageInfo.sharingMode =     VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.initialLayout =   VK_IMAGE_LAYOUT_UNDEFINED;

        m_Device =
            (Graphics::Vulkan::IDevice*)rendererAPI->GetDevice();

        if(m_Device->CreateImage(
            &imageInfo,
            m_FontImage) == SE_FALSE)
        {
            printf("Error in Vulkan!\n");
                
            return;
        }

        VkMemoryRequirements memReqs =
            m_Device->GetImageMemoryRequirements(
                m_FontImage
        );

		VkMemoryAllocateInfo memAllocInfo = {};

        memAllocInfo.sType =            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memAllocInfo.allocationSize =   memReqs.size;
		memAllocInfo.memoryTypeIndex =  m_Device->FindMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        m_Device->AllocateMemory(
            &memAllocInfo,
            m_FontMemory
        );
        m_Device->BindImageMemory(
            m_FontImage,
            m_FontMemory
        );

        VkImageViewCreateInfo viewInfo = {};

        viewInfo.sType =                        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image =                        m_FontImage;
		viewInfo.viewType =                     VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format =                       VK_FORMAT_R8G8B8A8_UNORM;
		viewInfo.subresourceRange.aspectMask =  VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.levelCount =  1;
		viewInfo.subresourceRange.layerCount =  1;

        m_Device->CreateImageView(
            &viewInfo,
            m_FontView
        );

    // TODO: Should be removed
        Graphics::Vulkan::IBuffer* stagingBuffer =
            (Graphics::Vulkan::IBuffer*)Reflection::CreateInstance("SteelEngine::Graphics::Vulkan::Buffer");

        stagingBuffer->CreateBuffer(
            m_Device,
            uploadSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        stagingBuffer->CopyBuffer(m_Device, fontData, uploadSize);

        VkCommandBuffer copyCommand;

        m_Device->AllocateCommandBuffer(
            copyCommand
        );

        VkCommandBufferBeginInfo beginInfo = {};

        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(copyCommand, &beginInfo);

        SetImageLayout(
            copyCommand,
            m_FontImage,
            VK_IMAGE_ASPECT_COLOR_BIT,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_PIPELINE_STAGE_HOST_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT
        );

        VkBufferImageCopy bufferCopyRegion = {};

		bufferCopyRegion.imageSubresource.aspectMask =  VK_IMAGE_ASPECT_COLOR_BIT;
		bufferCopyRegion.imageSubresource.layerCount =  1;
		bufferCopyRegion.imageExtent.width =            texWidth;
		bufferCopyRegion.imageExtent.height =           texHeight;
		bufferCopyRegion.imageExtent.depth =            1;

        vkCmdCopyBufferToImage(
			copyCommand,
			stagingBuffer->GetBuffer(),
			m_FontImage,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&bufferCopyRegion
		);

        // delete stagingBuffer;

        SetImageLayout(
            copyCommand,
            m_FontImage,
            VK_IMAGE_ASPECT_COLOR_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
        );

        m_Device->FlushCommandBuffer(
            copyCommand,
            rendererAPI->GetQueue()
        );
        m_Device->FreeCommandBuffer(
            copyCommand
        );

        VkSamplerCreateInfo samplerInfo = {};

        samplerInfo.sType =         VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.maxAnisotropy = 1.f;
		samplerInfo.magFilter =     VK_FILTER_LINEAR;
		samplerInfo.minFilter =     VK_FILTER_LINEAR;
		samplerInfo.mipmapMode =    VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.addressModeU =  VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeV =  VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeW =  VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.borderColor =   VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

		m_Device->CreateSampler(
            &samplerInfo,
            m_Sampler
        );

        VkDescriptorPoolSize descriptorPoolSize {};

        descriptorPoolSize.type =               VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorPoolSize.descriptorCount =    1;

        VkDescriptorPoolCreateInfo descriptorPoolInfo = {};

        descriptorPoolInfo.sType =          VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.poolSizeCount =  1;
        descriptorPoolInfo.pPoolSizes =     &descriptorPoolSize;
        descriptorPoolInfo.maxSets =        2;

        m_Device->CreateDescriptorPool(
            &descriptorPoolInfo,
            m_DescriptorPool
        );

        VkDescriptorSetLayoutBinding setLayoutBinding = {};

        setLayoutBinding.descriptorType =   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        setLayoutBinding.stageFlags =       VK_SHADER_STAGE_FRAGMENT_BIT;
        setLayoutBinding.binding =          0;
        setLayoutBinding.descriptorCount =  1;

        VkDescriptorSetLayoutCreateInfo descriptorLayout = {};

        descriptorLayout.sType =           VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorLayout.pBindings =       &setLayoutBinding;
        descriptorLayout.bindingCount =    1;

        m_Device->CreateDescriptorSetLayout(
            &descriptorLayout,
            m_DescriptorSetLayout
        );

        VkDescriptorSetAllocateInfo allocInfo = {};

        allocInfo.sType =               VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool =      m_DescriptorPool;
        allocInfo.pSetLayouts =         &m_DescriptorSetLayout;
        allocInfo.descriptorSetCount =  1;

        m_Device->AllocateDescriptorSet(
            &allocInfo,
            m_DescriptorSet
        );

        VkDescriptorImageInfo fontDescriptor = {};

        fontDescriptor.sampler =        m_Sampler;
        fontDescriptor.imageView =      m_FontView;
        fontDescriptor.imageLayout =    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkWriteDescriptorSet writeDescriptorSet = {};

        writeDescriptorSet.sType =              VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.dstSet =             m_DescriptorSet;
        writeDescriptorSet.descriptorType =     VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeDescriptorSet.dstBinding =         0;
        writeDescriptorSet.pImageInfo =         &fontDescriptor;
        writeDescriptorSet.descriptorCount =    1;

        m_Device->UpdateDescriptorSets(
            { writeDescriptorSet }
        );

        VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};

		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

        m_Device->CreatePipelineCache(
            &pipelineCacheCreateInfo,
            m_PipelineCache
        );

        VkPushConstantRange pushConstantRange = {};

        pushConstantRange.stageFlags =  VK_SHADER_STAGE_VERTEX_BIT;
        pushConstantRange.offset =      0;
        pushConstantRange.size =        sizeof(PushConstBlock);

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};

        pipelineLayoutCreateInfo.sType =                    VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount =           1;
        pipelineLayoutCreateInfo.pSetLayouts =              &m_DescriptorSetLayout;
        pipelineLayoutCreateInfo.pushConstantRangeCount =   1;
		pipelineLayoutCreateInfo.pPushConstantRanges =      &pushConstantRange;

        m_Device->CreatePipelineLayout(
            &pipelineLayoutCreateInfo,
            m_PipelineLayout
        );

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};

        inputAssemblyState.sType =                    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyState.topology =                 VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyState.flags =                    0;
        inputAssemblyState.primitiveRestartEnable =   VK_FALSE;

        VkPipelineRasterizationStateCreateInfo rasterizationState {};

        rasterizationState.sType =            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationState.polygonMode =      VK_POLYGON_MODE_FILL;
        rasterizationState.cullMode =         VK_CULL_MODE_NONE;
        rasterizationState.frontFace =        VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizationState.flags =            0;
        rasterizationState.depthClampEnable = VK_FALSE;
        rasterizationState.lineWidth =        1.0f;

        VkPipelineColorBlendAttachmentState blendAttachmentState = {};

		blendAttachmentState.blendEnable =          VK_TRUE;
		blendAttachmentState.colorWriteMask =       VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		blendAttachmentState.srcColorBlendFactor =  VK_BLEND_FACTOR_SRC_ALPHA;
		blendAttachmentState.dstColorBlendFactor =  VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		blendAttachmentState.colorBlendOp =         VK_BLEND_OP_ADD;
		blendAttachmentState.srcAlphaBlendFactor =  VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		blendAttachmentState.dstAlphaBlendFactor =  VK_BLEND_FACTOR_ZERO;
		blendAttachmentState.alphaBlendOp =         VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlendState = {};

        colorBlendState.sType =             VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendState.attachmentCount =   1;
        colorBlendState.pAttachments =      &blendAttachmentState;

        VkPipelineDepthStencilStateCreateInfo depthStencilState = {};

        depthStencilState.sType =               VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilState.depthTestEnable =     VK_FALSE;
        depthStencilState.depthWriteEnable =    VK_FALSE;
        depthStencilState.depthCompareOp =      VK_COMPARE_OP_LESS_OR_EQUAL;
        depthStencilState.front =               depthStencilState.back;
        depthStencilState.back.compareOp =      VK_COMPARE_OP_ALWAYS;

        VkPipelineViewportStateCreateInfo viewportState = {};

        viewportState.sType =         VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount =  1;
        viewportState.flags =         0;

        VkPipelineMultisampleStateCreateInfo multisampleState {};

        multisampleState.sType =                  VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleState.rasterizationSamples =   VK_SAMPLE_COUNT_1_BIT;
        multisampleState.flags =                  0;

        std::vector<VkDynamicState> dynamicStateEnables =
        {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

        VkPipelineDynamicStateCreateInfo dynamicState = {};

        dynamicState.sType =                VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.pDynamicStates =       dynamicStateEnables.data();
        dynamicState.dynamicStateCount =    dynamicStateEnables.size();
        dynamicState.flags =                0;

        m_Device->LoadShader(
            "ui",
            m_ImGUI_Shader,
            m_ShaderStages
        );

        VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};

        pipelineCreateInfo.sType =                  VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.layout =                 m_PipelineLayout;
        pipelineCreateInfo.renderPass =             rendererAPI->GetRenderPass();
        pipelineCreateInfo.flags =                  0;
        pipelineCreateInfo.basePipelineIndex =      -1;
        pipelineCreateInfo.basePipelineHandle =     VK_NULL_HANDLE;
		pipelineCreateInfo.pInputAssemblyState =    &inputAssemblyState;
		pipelineCreateInfo.pRasterizationState =    &rasterizationState;
		pipelineCreateInfo.pColorBlendState =       &colorBlendState;
		pipelineCreateInfo.pMultisampleState =      &multisampleState;
		pipelineCreateInfo.pViewportState =         &viewportState;
		pipelineCreateInfo.pDepthStencilState =     &depthStencilState;
		pipelineCreateInfo.pDynamicState =          &dynamicState;
		pipelineCreateInfo.stageCount =             m_ShaderStages.size();
		pipelineCreateInfo.pStages =                m_ShaderStages.data();

        VkVertexInputBindingDescription vInputBindDescription = {};

        vInputBindDescription.binding =     0;
        vInputBindDescription.stride =      sizeof(ImDrawVert);
        vInputBindDescription.inputRate =   VK_VERTEX_INPUT_RATE_VERTEX;

        std::vector<VkVertexInputAttributeDescription> vertexInputAttributes =
        {
			VertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, pos)),	// Location 0: Position
			VertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, uv)),	// Location 1: UV
			VertexInputAttributeDescription(0, 2, VK_FORMAT_R8G8B8A8_UNORM, offsetof(ImDrawVert, col)),	// Location 0: Color
		};

        VkPipelineVertexInputStateCreateInfo vertexInputState = {};
		
        vertexInputState.sType =                            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputState.vertexBindingDescriptionCount =    1;
		vertexInputState.pVertexBindingDescriptions =       &vInputBindDescription;
		vertexInputState.vertexAttributeDescriptionCount =  vertexInputAttributes.size();
		vertexInputState.pVertexAttributeDescriptions =     vertexInputAttributes.data();

        pipelineCreateInfo.pVertexInputState = &vertexInputState;

        m_Device->CreatePipelines(
            { pipelineCreateInfo },
            m_Pipeline,
            m_PipelineCache
        );

        m_VertexBuffer = (SteelEngine::Graphics::Vulkan::VertexBuffer*)Reflection::CreateInstance("SteelEngine::Graphics::Vulkan::VertexBuffer");
        m_IndexBuffer = (SteelEngine::Graphics::Vulkan::IndexBuffer*)Reflection::CreateInstance("SteelEngine::Graphics::Vulkan::IndexBuffer");

        m_Renderer->BindCommands(std::bind(&VulkanContext::PassCommands, this, std::placeholders::_1));
        m_Renderer->BindUpdateUniforms(std::bind(&VulkanContext::UpdateBuffers, this));

        printf("Vulkan context init successful!\n");
    }

    void VulkanContext::Update()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL2_NewFrame((SDL_Window*)m_Window->GetWindow());
    }

    void VulkanContext::UploadDrawData()
    {

    }

    void VulkanContext::ProcessEvent(const void* event)
    {
        ImGui_ImplSDL2_ProcessEvent((SDL_Event*)event);
    }

}