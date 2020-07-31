#pragma once

#include "Graphics/IContext.h"

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_vulkan.h"

#include "RuntimeReflection/Macro.h"

#include "Editor/ImGUI/ImGUI_Editor.h"

#include "Editor/ImGUI/VulkanContext.Generated.h"

#include "queue"

#include "Platform/Graphics/Vulkan/VertexBuffer.h"
#include "Platform/Graphics/Vulkan/IndexBuffer.h"
#include "Platform/Graphics/Vulkan/RenderPass.h"
#include "Platform/Graphics/Vulkan/Framebuffer.h"
#include "Platform/Graphics/Vulkan/DescriptorPool.h"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::HOT_RELOAD,
        Editor::ImGUI::ImGUI_Editor::API::CONTEXT_API = Graphics::IRenderer::API::VULKAN
    )
    class VulkanContext : public IContext
    {
        GENERATED_BODY
    public:
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

    private:
        struct PushConstBlock
        {
            glm::vec2 m_Scale;
            glm::vec2 m_Translate;
        } m_PushConstBlock;

        ImGuiContext* m_Context;
        IWindow* m_Window;
        const void* m_RenderDevice;

        Graphics::ITexture* texture;

        VkDescriptorSet         m_DescriptorSet2;
        Platform::Vulkan::DescriptorPool* m_DescriptorPool;
        Platform::Vulkan::Pipeline* m_Pipeline;
        std::vector<Platform::Vulkan::Framebuffer*> m_SwapChainFramebuffers;

        Platform::Vulkan::RenderPass* m_RenderPass;

        Platform::Vulkan::VertexBuffer* m_VertexBuffer;
        Platform::Vulkan::IndexBuffer* m_IndexBuffer;

        std::queue<VkBuffer> m_BuffersToClean;
        std::queue<VkDeviceMemory> m_MemoryToClean;

        int32_t m_VertexCount = 0;
	    int32_t m_IndexCount = 0;

        void PassCommands(void*, uint32_t);
        void UpdateBuffers();

    public:
        VulkanContext();
        ~VulkanContext();

        void Init(const IReflectionData* windowType, IWindow* window, const Graphics::RenderDevice* renderDevice) override;
        void Update() override;
        void UploadDrawData() override;
        void ProcessEvent(const void* event) override;

        std::function<void(void*, uint32_t)> GetEditorCommands() override { return std::bind(&VulkanContext::PassCommands, this, std::placeholders::_1, std::placeholders::_2); }
        std::function<void()> GetUpdate() override { return std::bind(&VulkanContext::UpdateBuffers, this); }

        SE_METHOD()
        inline void* GetContext() { return m_Context; }

        SE_METHOD()
        inline void MakeCurrent() { ImGui::SetCurrentContext(m_Context); }
    };

}