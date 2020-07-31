#pragma once

#include "Platform/Graphics/Vulkan/LogicalDevice.h"
#include "Platform/Graphics/Vulkan/RenderPass.h"

#include "glm/glm.hpp"

#include "array"
#include "vector"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    class CommandBuffer;
    class DescriptorPool;

    std::vector<char> readFile(const std::string& filename);

    class Pipeline
    {
        friend class CommandBuffer;
        friend class DescriptorPool;
    private:
        VkPipelineCache     m_PipelineCache;
        VkPipelineLayout    m_PipelineLayout;
        VkPipeline          m_Pipeline;

        RenderPass* m_RenderPass;
        VkDescriptorSetLayout m_DescriptorSetLayout;

        static VkShaderModule CreateShaderModule(LogicalDevice* lDevice, const std::vector<char>& code);

    public:
        struct CreateInfo
        {
        private:
            uint32_t m_SamplerBindingIndex = 0;

        public:
            std::vector<VkDescriptorSetLayoutBinding> m_DescriptorSetLayoutBindings;
            std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
            std::vector<VkPipelineVertexInputStateCreateInfo> m_VertexInputStates;
            VkPipelineInputAssemblyStateCreateInfo m_InputAssemblyStateCreateInfo;
            VkPipelineViewportStateCreateInfo m_ViewportState;
            VkPipelineRasterizationStateCreateInfo m_Rasterizer;
            VkPipelineMultisampleStateCreateInfo m_Multisampling;
            std::vector<VkPipelineColorBlendAttachmentState> m_ColorBlendAttachments;
            VkPipelineColorBlendStateCreateInfo m_ColorBlendState;
            VkPipelineDynamicStateCreateInfo* m_DynamicState;
            std::vector<VkPushConstantRange> m_PushConstantRanges;
            VkPipelineDepthStencilStateCreateInfo* m_DepthStencilState;

            CreateInfo()
            {
                m_InputAssemblyStateCreateInfo = {};
                m_ViewportState = {};
                m_Rasterizer = {};
                m_Multisampling = {};
                m_ColorBlendState = {};
                m_DynamicState = 0;
                m_DepthStencilState = 0;
            }

            void AddShaderStage(LogicalDevice* lDevice, const char* filename, const VkShaderStageFlagBits& stageType)
            {
                VkPipelineShaderStageCreateInfo shaderStageInfo = {};
                shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                shaderStageInfo.stage = stageType;
                shaderStageInfo.module = CreateShaderModule(lDevice, readFile(filename));
                shaderStageInfo.pName = "main";

                m_ShaderStages.push_back(shaderStageInfo);
            }

            void AddUniformSampler(uint32_t descriptorCount, const VkDescriptorType& descriptorType, const VkShaderStageFlags& stageFlags)
            {
                VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
                samplerLayoutBinding.binding = m_SamplerBindingIndex++;
                samplerLayoutBinding.descriptorCount = descriptorCount;
                samplerLayoutBinding.descriptorType = descriptorType;
                samplerLayoutBinding.pImmutableSamplers = nullptr;
                samplerLayoutBinding.stageFlags = stageFlags;

                m_DescriptorSetLayoutBindings.push_back(samplerLayoutBinding);
            }

            void AddInput(VkVertexInputAttributeDescription* vertexInputAttributeDescriptions, uint32_t vertexInputAttributeDescriptionsCount, const VkVertexInputBindingDescription& binding)
            {
                VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
                vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vertexInputInfo.vertexBindingDescriptionCount = 1;
                vertexInputInfo.vertexAttributeDescriptionCount = vertexInputAttributeDescriptionsCount;
                vertexInputInfo.pVertexBindingDescriptions = &binding;
                vertexInputInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions;

                m_VertexInputStates.push_back(vertexInputInfo);
            }

            void SetInputAssemblyStateCreateInfo(
                const VkPrimitiveTopology& topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                const VkBool32& primitiveRestartEnable = VK_FALSE)
            {
                m_InputAssemblyStateCreateInfo = {};
                m_InputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                m_InputAssemblyStateCreateInfo.topology = topology;
                m_InputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
            }

            void SetViewport(const VkViewport& viewport, const VkRect2D& scissor)
            {
                m_ViewportState = {};
                m_ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                m_ViewportState.viewportCount = 1;
                m_ViewportState.pViewports = &viewport;
                m_ViewportState.scissorCount = 1;
                m_ViewportState.pScissors = &scissor;
            }

            void SetViewport(uint32_t vierportCount, uint32_t scissorCount)
            {
                m_ViewportState = {};
                m_ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                m_ViewportState.viewportCount = vierportCount;
                m_ViewportState.scissorCount = scissorCount;
            }

            void SetRasterizer(
                const VkPolygonMode& polygonMode = VK_POLYGON_MODE_FILL,
                const VkCullModeFlags& cullMode = VK_CULL_MODE_BACK_BIT,
                const VkFrontFace& frontFace = VK_FRONT_FACE_CLOCKWISE)
            {
                m_Rasterizer = {};
                m_Rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                m_Rasterizer.depthClampEnable = VK_FALSE;
                m_Rasterizer.rasterizerDiscardEnable = VK_FALSE;
                m_Rasterizer.polygonMode = polygonMode;
                m_Rasterizer.lineWidth = 1.0f;
                m_Rasterizer.cullMode = cullMode;
                m_Rasterizer.frontFace = frontFace;
                m_Rasterizer.depthBiasEnable = VK_FALSE;
                m_Rasterizer.depthBiasConstantFactor = 0.0f; // Optional
                m_Rasterizer.depthBiasClamp = 0.0f; // Optional
                m_Rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
            }

            void SetMultisampling()
            {
                m_Multisampling = {};
                m_Multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                m_Multisampling.sampleShadingEnable = VK_FALSE;
                m_Multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
                m_Multisampling.minSampleShading = 1.0f; // Optional
                m_Multisampling.pSampleMask = nullptr; // Optional
                m_Multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
                m_Multisampling.alphaToOneEnable = VK_FALSE; // Optional
            }

            void AddColorBlendAttachment(
                const VkColorComponentFlags& colorWriteMask,
                const VkBool32& blendEnable,
                const VkBlendFactor& srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
                const VkBlendFactor& dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
                const VkBlendOp& colorBlendOp = VK_BLEND_OP_ADD,
                const VkBlendFactor& srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                const VkBlendFactor& dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                const VkBlendOp& alphaBlendOp = VK_BLEND_OP_ADD)
            {
                VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
                colorBlendAttachment.colorWriteMask = colorWriteMask;
                colorBlendAttachment.blendEnable = blendEnable;
                colorBlendAttachment.srcColorBlendFactor = srcColorBlendFactor; // Optional
                colorBlendAttachment.dstColorBlendFactor = dstColorBlendFactor; // Optional
                colorBlendAttachment.colorBlendOp = colorBlendOp; // Optional
                colorBlendAttachment.srcAlphaBlendFactor = srcAlphaBlendFactor; // Optional
                colorBlendAttachment.dstAlphaBlendFactor = dstAlphaBlendFactor; // Optional
                colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

                m_ColorBlendAttachments.push_back(colorBlendAttachment);
            }

            void SetColorBlendState()
            {
                m_ColorBlendState = {};
                m_ColorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                m_ColorBlendState.logicOpEnable = VK_FALSE;
                m_ColorBlendState.logicOp = VK_LOGIC_OP_COPY; // Optional
                m_ColorBlendState.attachmentCount = static_cast<uint32_t>(m_ColorBlendAttachments.size());
                m_ColorBlendState.pAttachments = m_ColorBlendAttachments.data();
                m_ColorBlendState.blendConstants[0] = 0.0f; // Optional
                m_ColorBlendState.blendConstants[1] = 0.0f; // Optional
                m_ColorBlendState.blendConstants[2] = 0.0f; // Optional
                m_ColorBlendState.blendConstants[3] = 0.0f; // Optional
            }

            void SetDynamicState(const std::vector<VkDynamicState>& dynamicStates)
            {
                m_DynamicState = new VkPipelineDynamicStateCreateInfo();
                *m_DynamicState = {};
                m_DynamicState->sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                m_DynamicState->dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
                m_DynamicState->pDynamicStates = dynamicStates.data();
            }

            template <typename A>
            void AddConstantRange(uint32_t offset = 0)
            {
                VkPushConstantRange pushConstantRange = {};

                pushConstantRange.stageFlags =  VK_SHADER_STAGE_VERTEX_BIT;
                pushConstantRange.offset =      offset;
                pushConstantRange.size =        sizeof(A);

                m_PushConstantRanges.push_back(pushConstantRange);
            }

            void SetDepthStencil()
            {
                m_DepthStencilState = new VkPipelineDepthStencilStateCreateInfo();
                *m_DepthStencilState = {};
                m_DepthStencilState->sType =               VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
                m_DepthStencilState->depthTestEnable =     VK_FALSE;
                m_DepthStencilState->depthWriteEnable =    VK_FALSE;
                m_DepthStencilState->depthCompareOp =      VK_COMPARE_OP_LESS_OR_EQUAL;
                m_DepthStencilState->front =               m_DepthStencilState->back;
                m_DepthStencilState->back.compareOp =      VK_COMPARE_OP_ALWAYS;
            }
        };

    private:
    // It is for descriptor pool
        CreateInfo m_CreateInfo;

    public:
        Pipeline(RenderPass* renderPass);
        ~Pipeline();

        bool SetupPipeline(LogicalDevice* lDevice, CreateInfo createInfo);
    };

}}}