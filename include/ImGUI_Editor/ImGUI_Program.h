#pragma once

#include "Graphics/Vulkan/IProgram.h"
#include "Graphics/Vulkan/Shader.h"

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_vulkan.h"
#include "imgui/examples/imgui_impl_sdl.h"

#include "glm/glm.hpp"

#include "Vulkan/vulkan.h"

#include "Graphics/Vulkan/VertexBuffer.h"
#include "Graphics/Vulkan/IndexBuffer.h"

#include "Graphics/IEditor.h"

#include "queue"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    /*
    class ImGUI_Program : public IProgram
    {
    public:
        struct PushConstBlock
        {
            glm::vec2 m_Scale;
            glm::vec2 m_Translate;
        } m_PushConstBlock;

    private:
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

        int32_t m_VertexCount = 0;
	    int32_t m_IndexCount = 0;

        VertexBuffer* m_VertexBuffer;
        IndexBuffer* m_IndexBuffer;

        const Device* m_Device;

        std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
        Shader* m_ImGUI_Shader;

        IProgramUserData* m_UData;
        Editor::IEditor** m_Editor;
        IWindow* m_Window;

    public:
        ImGUI_Program();
        ~ImGUI_Program();

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

        Result Init(
            const Device* device,
            VkQueue queue,
            VkRenderPass renderPass,
            IProgramUserData* data) override;
        void Render(VkCommandBuffer commandBuffer) override;
        void DrawUI() override;
        void UpdateBuffers() override;
    };
    */

}}}