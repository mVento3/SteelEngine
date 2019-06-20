#include "Rendering/Vulkan/GraphicsPipeline.h"

#include "Rendering/Vulkan/Renderer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    GraphicsPipeline::GraphicsPipeline()
    {

    }

    GraphicsPipeline::~GraphicsPipeline()
    {

    }

    Result GraphicsPipeline::Create(Renderer* renderer)
    {
        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};

        vertexInputInfo.sType =                             VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount =     0;
        vertexInputInfo.vertexAttributeDescriptionCount =   0;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};

        inputAssembly.sType =                   VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology =                VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable =  VK_FALSE;

        VkViewport viewport = {};

        viewport.x =        0.0f;
        viewport.y =        0.0f;
        viewport.width =    (float)renderer->m_SwapChain->m_SwapChainExtent.width;
        viewport.height =   (float)renderer->m_SwapChain->m_SwapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};

        scissor.offset = { 0, 0 };
        scissor.extent = renderer->m_SwapChain->m_SwapChainExtent;

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
        rasterizer.frontFace =                  VK_FRONT_FACE_CLOCKWISE;
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

        pipelineLayoutInfo.sType =                  VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount =         0;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        if(vkCreatePipelineLayout(renderer->m_LogicalDevice->GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        m_SomeShader = new Shader(renderer->m_LogicalDevice);

        std::vector<VkPipelineShaderStageCreateInfo> stages;
        std::vector<VkShaderModule> modules;

        m_SomeShader->LoadShader("", stages, modules);

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
        pipelineInfo.renderPass =           renderer->m_RenderPass->GetRenderPass();
        pipelineInfo.subpass =              0;
        pipelineInfo.basePipelineHandle =   VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex =    -1; // Optional

        if(vkCreateGraphicsPipelines(renderer->m_LogicalDevice->GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        vkDestroyShaderModule(renderer->m_LogicalDevice->GetLogicalDevice(), modules[1], nullptr);
        vkDestroyShaderModule(renderer->m_LogicalDevice->GetLogicalDevice(), modules[0], nullptr);

        return SE_TRUE;
    }

    void GraphicsPipeline::Cleanup(Renderer* renderer)
    {
        vkDestroyPipeline(renderer->m_LogicalDevice->GetLogicalDevice(), m_GraphicsPipeline, nullptr);
        vkDestroyPipelineLayout(renderer->m_LogicalDevice->GetLogicalDevice(), m_PipelineLayout, nullptr);
    }

}}}