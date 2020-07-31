#include "Editor/ImGUI/VulkanContext.h"

#include "RuntimeReflection/Reflection.h"

#include "Logger/Logger.h"

#include "Core/Core.h"

#include "Utils/Graphics/Texture.h"

#include "Platform/Graphics/Vulkan/Vulkan.h"
#include "Platform/Graphics/Vulkan/CommandBuffer.h"

#undef max

namespace SteelEngine {

    void VulkanContext::PassCommands(void* commandBuffer, uint32_t index)
    {
        ImGuiIO& io = ImGui::GetIO();
        Platform::Vulkan::CommandBuffer* cmd = (Platform::Vulkan::CommandBuffer*)commandBuffer;

        cmd->BeginRenderPass(m_RenderPass, m_SwapChainFramebuffers[index], { 1920, 1080 }, {{ 0.f, 0.f, 0.f, 1.f }});
        cmd->BindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);

        VkViewport viewport = {};

        viewport.width =    io.DisplaySize.x;
        viewport.height =   io.DisplaySize.y;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        cmd->SetViewport(
            0,
            { viewport }
        );

        m_PushConstBlock.m_Scale = glm::vec2(2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y);
		m_PushConstBlock.m_Translate = glm::vec2(-1.0f);

        cmd->PushConstants(
            m_Pipeline,
            VK_SHADER_STAGE_VERTEX_BIT,
            0,
            sizeof(PushConstBlock),
            &m_PushConstBlock
        );

        ImDrawData* imDrawData = ImGui::GetDrawData();

		int32_t vertexOffset = 0;
		int32_t indexOffset = 0;

        if(imDrawData->CmdListsCount > 0)
        {
            cmd->BindVertexBuffers(0, { m_VertexBuffer->GetBuffer() }, { 0 });
            cmd->BindIndexBuffer(m_IndexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);

			for(int32_t i = 0; i < imDrawData->CmdListsCount; i++)
			{
				const ImDrawList* cmd_list = imDrawData->CmdLists[i];

				for(int32_t j = 0; j < cmd_list->CmdBuffer.Size; j++)
				{
					const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[j];
                    VkRect2D scissorRect;

                    if(pcmd->TextureId)
                    {
                        cmd->BindDescriptorSet(
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_Pipeline,
                            0,
                            (VkDescriptorSet)pcmd->TextureId
                        );
                    }
                    else
                    {
                        cmd->BindDescriptorSets(
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_Pipeline,
                            0,
                            m_DescriptorPool,
                            { }
                        );
                    }

                    scissorRect.offset.x = std::max((int32_t)(pcmd->ClipRect.x), 0);
                    scissorRect.offset.y = std::max((int32_t)(pcmd->ClipRect.y), 0);
                    scissorRect.extent.width = (uint32_t)(pcmd->ClipRect.z - pcmd->ClipRect.x);
                    scissorRect.extent.height = (uint32_t)(pcmd->ClipRect.w - pcmd->ClipRect.y);

                    cmd->SetScissor(
                        0,
                        { scissorRect }
                    );
                    cmd->DrawIndexed(
                        pcmd->ElemCount,
                        1,
                        indexOffset,
                        vertexOffset,
                        0
                    );

                    indexOffset += pcmd->ElemCount;
				}

				vertexOffset += cmd_list->VtxBuffer.Size;
			}
		}

        cmd->EndRenderPass();
    }

    void VulkanContext::UpdateBuffers()
    {
        const Graphics::Vulkan* vk = (Graphics::Vulkan*)m_RenderDevice;

        while(!m_BuffersToClean.empty())
        {
            vk->m_LDevice->DestroyBuffer(m_BuffersToClean.front());

            m_BuffersToClean.pop();
        }

        while(!m_MemoryToClean.empty())
        {
            vk->m_LDevice->FreeMemory(m_MemoryToClean.front());

            m_MemoryToClean.pop();
        }

        ImDrawData* imDrawData = ImGui::GetDrawData();

		VkDeviceSize vertexBufferSize = imDrawData->TotalVtxCount * sizeof(ImDrawVert);
		VkDeviceSize indexBufferSize = imDrawData->TotalIdxCount * sizeof(ImDrawIdx);

        if((vertexBufferSize == 0) || (indexBufferSize == 0))
        {
			return;
		}

        if(m_VertexBuffer == 0 || m_VertexCount != imDrawData->TotalVtxCount)
        {
            if(m_VertexBuffer->GetBuffer())
            {
                m_BuffersToClean.push(m_VertexBuffer->GetBuffer());
            }

            if(m_VertexBuffer->GetMemory())
            {
                m_MemoryToClean.push(m_VertexBuffer->GetMemory());
            }

            m_VertexBuffer->Unmap();
            m_VertexBuffer->CreateBuffer(
                vertexBufferSize,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );

            m_VertexCount = imDrawData->TotalVtxCount;

            m_VertexBuffer->Map();
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

            m_IndexBuffer->Unmap();
            m_IndexBuffer->CreateBuffer(
                vertexBufferSize,
                VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );

            m_IndexCount = imDrawData->TotalIdxCount;

            m_IndexBuffer->Map();
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

        m_VertexBuffer->Flush();
		m_IndexBuffer->Flush();
    }

    VulkanContext::VulkanContext()
    {
        m_DescriptorPool = VK_NULL_HANDLE;
    }

    VulkanContext::~VulkanContext()
    {

    }

    void VulkanContext::Init(const IReflectionData* windowType, IWindow* window, const Graphics::RenderDevice* renderDevice)
    {
        m_Window = window;
        m_RenderDevice = renderDevice;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForVulkan((SDL_Window*)window->GetWindow());

        m_Context = ImGui::GetCurrentContext();

        Graphics::Vulkan* vk = (Graphics::Vulkan*)renderDevice;

        unsigned char* fontData;
		int texWidth;
        int texHeight;

		io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);

		VkDeviceSize uploadSize = texWidth * texHeight * 4 * sizeof(char);

        texture = Utils::Graphics::Texture::Create();

        renderDevice->GenerateTexture(texture);
        renderDevice->TextureImage2D(texture, (Graphics::InternalFormat)0, texWidth, texHeight, (Graphics::Format)0, (Graphics::DataType)0, fontData);

        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = vk->m_Surface->GetSwapChainFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        m_RenderPass = new Platform::Vulkan::RenderPass(vk->m_LDevice);

        Platform::Vulkan::RenderPass::CreateInfo rCreateInfo;

        rCreateInfo.AddAttachment(colorAttachment);
        rCreateInfo.AddDependency(dependency);
        rCreateInfo.AddSubpass(subpass);

        m_RenderPass->SetupRenderPass(rCreateInfo);

        m_SwapChainFramebuffers.resize(vk->m_SwapChainImageViews.size());

        for(size_t i = 0; i < m_SwapChainFramebuffers.size(); i++)
        {
            Platform::Vulkan::Framebuffer* fb = new Platform::Vulkan::Framebuffer(m_RenderPass, { 1920, 1080 });

            Platform::Vulkan::Framebuffer::CreateInfo createInfo;

            createInfo.AddAttchamnet(vk->m_SwapChainImageViews[i]);

            fb->SetupFramebuffer(vk->m_LDevice, createInfo);

            m_SwapChainFramebuffers[i] = fb;
        }

        m_Pipeline = new Platform::Vulkan::Pipeline(m_RenderPass);

        Platform::Vulkan::Pipeline::CreateInfo createInfioP;

        createInfioP.AddConstantRange<PushConstBlock>();
        createInfioP.AddUniformSampler(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
        createInfioP.SetInputAssemblyStateCreateInfo();
        createInfioP.SetRasterizer(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        createInfioP.AddColorBlendAttachment(
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
            VK_TRUE,
            VK_BLEND_FACTOR_SRC_ALPHA,
            VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            VK_BLEND_OP_ADD,
            VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            VK_BLEND_FACTOR_ZERO,
            VK_BLEND_OP_ADD
        );
        createInfioP.SetColorBlendState();
        createInfioP.SetDepthStencil();
        createInfioP.SetViewport(1, 1);
        createInfioP.SetMultisampling();

        std::vector<VkDynamicState> dynamicStateEnables =
        {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

        createInfioP.SetDynamicState(dynamicStateEnables);
        createInfioP.AddShaderStage(vk->m_LDevice, "D:/Projects/C++/SteelEngine/bin/Resources/Shaders/ui.vs", VK_SHADER_STAGE_VERTEX_BIT);
        createInfioP.AddShaderStage(vk->m_LDevice, "D:/Projects/C++/SteelEngine/bin/Resources/Shaders/ui.fs", VK_SHADER_STAGE_FRAGMENT_BIT);
        
        VkVertexInputBindingDescription vInputBindDescription = {};

        vInputBindDescription.binding =     0;
        vInputBindDescription.stride =      sizeof(ImDrawVert);
        vInputBindDescription.inputRate =   VK_VERTEX_INPUT_RATE_VERTEX;

        std::array<VkVertexInputAttributeDescription, 3> vertexInputAttributes =
        {
			VertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, pos)),	// Location 0: Position
			VertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, uv)),	// Location 1: UV
			VertexInputAttributeDescription(0, 2, VK_FORMAT_R8G8B8A8_UNORM, offsetof(ImDrawVert, col)),	// Location 0: Color
		};

        createInfioP.AddInput(vertexInputAttributes.data(), 3, vInputBindDescription);

        m_Pipeline->SetupPipeline(vk->m_LDevice, createInfioP);

        m_DescriptorPool = new Platform::Vulkan::DescriptorPool();

        Platform::Vulkan::Texture* tex = (Platform::Vulkan::Texture*)(*texture->GetTextureID_Ptr());

        m_DescriptorPool->SetupDescriptorPool(vk->m_LDevice, m_Pipeline, 1, 2);
        m_DescriptorPool->SetupDescriptorSets(vk->m_LDevice, m_Pipeline, 1, tex);

        VkDescriptorImageInfo viewportInfo;

        vk->Test(&viewportInfo, &m_DescriptorSet2);

        m_DescriptorPool->SetupDescriptorSet(vk->m_LDevice, m_Pipeline, viewportInfo, m_DescriptorSet2);

        m_IndexBuffer = new Platform::Vulkan::IndexBuffer(vk->m_PDevice, vk->m_LDevice);
        m_VertexBuffer = new Platform::Vulkan::VertexBuffer(vk->m_PDevice, vk->m_LDevice);

        windowType->Invoke("MakeCurrent", window);
    }

    void VulkanContext::Update()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL2_NewFrame((SDL_Window*)m_Window->GetWindow());
    }

    void VulkanContext::UploadDrawData()
    {
        // printf("AAAA\n");
    }

    void VulkanContext::ProcessEvent(const void* event)
    {
        ImGui_ImplSDL2_ProcessEvent((const SDL_Event*)event);
    }

}