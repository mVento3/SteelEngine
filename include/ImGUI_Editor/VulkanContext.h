#pragma once

#include "Graphics/IContext.h"

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_vulkan.h"

#include "Graphics/Vulkan/DeepLayer/Device.h"
#include "Graphics/Vulkan/DeepLayer/CommandPool.h"

#include "Graphics/Vulkan/VertexBuffer.h"
#include "Graphics/Vulkan/IndexBuffer.h"
#include "Graphics/Vulkan/Buffer.h"

#include "Graphics/IRendererVulkan.h"

#include "glm/glm.hpp"

#include "ImGUI_Editor/VulkanContext.Generated.h"

#include "queue"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class VulkanContext : public IContext
    {
        GENERATED_BODY
    public:
        struct PushConstBlock
        {
            glm::vec2 m_Scale;
            glm::vec2 m_Translate;
        } m_PushConstBlock;

    private:
        ImGuiContext* m_Context;
        IWindow* m_Window;

        Graphics::IRendererAPI<Graphics::IRenderer::API::VULKAN_API>* m_Renderer;

        VkImage                 m_FontImage;
        VkDeviceMemory          m_FontMemory;
        VkImageView             m_FontView;
        VkSampler               m_Sampler;
        VkDescriptorPool        m_DescriptorPool;
        VkDescriptorSetLayout   m_DescriptorSetLayout;
        VkDescriptorSet         m_DescriptorSet;
        VkPipelineCache         m_PipelineCache;
        VkPipelineLayout        m_PipelineLayout;
        VkPipeline              m_Pipeline;

        Graphics::Vulkan::VertexBuffer* m_VertexBuffer;
        Graphics::Vulkan::IndexBuffer* m_IndexBuffer;

        std::queue<VkBuffer> m_BuffersToClean;
        std::queue<VkDeviceMemory> m_MemoryToClean;

        int32_t m_VertexCount = 0;
	    int32_t m_IndexCount = 0;

        std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
        Graphics::Vulkan::IShader* m_ImGUI_Shader;

        const Graphics::Vulkan::IDevice* m_Device;
        const Graphics::Vulkan::CommandPool* m_CommandPool;

        void PassCommands(Graphics::Vulkan::ICommandBuffer* commandBuffer);
        void UpdateBuffers();

    public:
        VulkanContext();
        ~VulkanContext();

        static Result SetImageLayout(
            VkCommandBuffer cmdbuffer,
			VkImage image,
			VkImageAspectFlags aspectMask,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout,
			VkPipelineStageFlags srcStageMask,
			VkPipelineStageFlags dstStageMask
        );
        static Result SetImageLayout(
            VkCommandBuffer cmdbuffer,
			VkImage image,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout,
			VkImageSubresourceRange subresourceRange,
			VkPipelineStageFlags srcStageMask,
			VkPipelineStageFlags dstStageMask
        );
        static VkVertexInputAttributeDescription VertexInputAttributeDescription(
            uint32_t binding,
			uint32_t location,
			VkFormat format,
			uint32_t offset
        );

        void Init(IWindow* window, Graphics::IRenderer* renderer) override;
        void Update() override;
        void UploadDrawData() override;
        void ProcessEvent(const void* event) override;

        SE_METHOD()
        inline void* GetContext() { return m_Context; }
    };

}