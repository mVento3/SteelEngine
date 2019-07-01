#include "Rendering/Vulkan/GraphicsPipeline.h"

#include "Rendering/Vulkan/SwapChain.h"
#include "Rendering/Vulkan/RenderPass.h"
#include "Rendering/Vulkan/Buffer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    GraphicsPipeline::GraphicsPipeline()
    {
        
    }

    GraphicsPipeline::~GraphicsPipeline()
    {

    }

    Result GraphicsPipeline::Create(
        const LogicalDevice& logicalDevice,
        const SwapChain& swapChain,
        const RenderPass& renderPass,
        const DescriptorSetLayout& desc,
        std::vector<VkPipelineShaderStageCreateInfo> stages)
    {
        auto bindingDescription = Vertex::GetBindingDescription();
        auto attributeDescription = Vertex::GetAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};

        vertexInputInfo.sType =                             VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount =     1;
        vertexInputInfo.pVertexBindingDescriptions =        &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount =   attributeDescription.size();
        vertexInputInfo.pVertexAttributeDescriptions =      attributeDescription.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};

        inputAssembly.sType =                   VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology =                VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable =  VK_FALSE;

        VkViewport viewport = {};

        viewport.x =        0.0f;
        viewport.y =        0.0f;
        viewport.width =    (float)swapChain.m_SwapChainExtent.width;
        viewport.height =   (float)swapChain.m_SwapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};

        scissor.offset = { 0, 0 };
        scissor.extent = swapChain.m_SwapChainExtent;

        VkPipelineViewportStateCreateInfo viewportState = {};

        viewportState.sType =           VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount =   1;
        viewportState.pViewports =      &viewport;
        viewportState.scissorCount =    1;
        viewportState.pScissors =       &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer = {};

        rasterizer.sType =                      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable =           VK_FALSE;
        rasterizer.rasterizerDiscardEnable =    VK_FALSE;
        rasterizer.polygonMode =                VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth =                  1.0f;
        rasterizer.cullMode =                   VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace =                  VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable =            VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling = {};

        multisampling.sType =                   VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable =     VK_FALSE;
        multisampling.rasterizationSamples =    VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};

        colorBlendAttachment.colorWriteMask =   VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable =      VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending = {};

        colorBlending.sType =           VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable =   VK_FALSE;
        colorBlending.logicOp =         VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments =    &colorBlendAttachment;

        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};

        pipelineLayoutInfo.sType =          VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts =    &desc.m_DescriptorSetLayout;

        if(vkCreatePipelineLayout(logicalDevice.GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        VkGraphicsPipelineCreateInfo pipelineInfo = {};

        pipelineInfo.sType =                VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount =           2;
        pipelineInfo.pStages =              stages.data();
        pipelineInfo.pVertexInputState =    &vertexInputInfo;
        pipelineInfo.pInputAssemblyState =  &inputAssembly;
        pipelineInfo.pViewportState =       &viewportState;
        pipelineInfo.pRasterizationState =  &rasterizer;
        pipelineInfo.pMultisampleState =    &multisampling;
        pipelineInfo.pDepthStencilState =   nullptr; // Optional
        pipelineInfo.pColorBlendState =     &colorBlending;
        pipelineInfo.pDynamicState =        nullptr; // Optional
        pipelineInfo.layout =               m_PipelineLayout;
        pipelineInfo.renderPass =           renderPass.GetRenderPass();
        pipelineInfo.subpass =              0;
        pipelineInfo.basePipelineHandle =   VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex =    -1; // Optional

        if(vkCreateGraphicsPipelines(logicalDevice.GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    void GraphicsPipeline::Cleanup(const LogicalDevice& logicalDevice, const SwapChain& swapChain)
    {
        vkDestroyPipeline(logicalDevice.GetLogicalDevice(), m_GraphicsPipeline, nullptr);
        vkDestroyPipelineLayout(logicalDevice.GetLogicalDevice(), m_PipelineLayout, nullptr);

        for(size_t i = 0; i < swapChain.m_SwapChainImages.size(); i++)
        {
            vkDestroyBuffer(logicalDevice.GetLogicalDevice(), m_UniformBuffers[i], nullptr);
            vkFreeMemory(logicalDevice.GetLogicalDevice(), m_UniformBuffersMemory[i], nullptr);
        }
    }

    Result GraphicsPipeline::CreateUniformBuffers(const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, const SwapChain& swapChain, Type::uint32 size)
    {
        m_UniformBuffers.resize(swapChain.m_SwapChainImages.size());
        m_UniformBuffersMemory.resize(swapChain.m_SwapChainImages.size());

        for(size_t i = 0; i < swapChain.m_SwapChainImages.size(); i++)
        {
            Buffer::CreateBuffer(
                physicalDevice,
                logicalDevice,
                size,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                m_UniformBuffers[i],
                m_UniformBuffersMemory[i]
            );
        }

        return SE_TRUE;
    }

}}}