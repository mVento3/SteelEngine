#include "Platform/Graphics/Vulkan/Pipeline.h"

#include "fstream"

namespace SteelEngine { namespace Platform { namespace Vulkan {

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

    VkShaderModule Pipeline::CreateShaderModule(LogicalDevice* lDevice, const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;

        lDevice->CreateShaderModule(createInfo, &shaderModule);

        return shaderModule;
    }

    Pipeline::Pipeline(RenderPass* renderPass) :
        m_RenderPass(renderPass)
    {
        m_PipelineLayout = 0;
        m_DescriptorSetLayout = 0;
        m_Pipeline = 0;
    }

    Pipeline::~Pipeline()
    {

    }

    bool Pipeline::SetupPipeline(LogicalDevice* lDevice, CreateInfo createInfo)
    {
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = createInfo.m_DescriptorSetLayoutBindings.size();
        layoutInfo.pBindings = createInfo.m_DescriptorSetLayoutBindings.data();

        if(!lDevice->CreateDescriptorSetLayout(layoutInfo, &m_DescriptorSetLayout))
        {
            printf("Failed to create descriptor set layout!\n");

            return false;
        }

        m_CreateInfo = createInfo;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(createInfo.m_PushConstantRanges.size()); // Optional
        pipelineLayoutInfo.pPushConstantRanges = createInfo.m_PushConstantRanges.data(); // Optional

        if(!lDevice->CreatePipelineLayout(pipelineLayoutInfo, &m_PipelineLayout))
        {
            printf("Failed to create pipeline layout!\n");\

            for(VkPipelineShaderStageCreateInfo& stage : createInfo.m_ShaderStages)
            {
                vkDestroyShaderModule(lDevice->GetLogicalDevice(), stage.module, nullptr); 
            }

            return false;
        }

        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = static_cast<uint32_t>(createInfo.m_ShaderStages.size());
        pipelineInfo.pStages = createInfo.m_ShaderStages.data();
        pipelineInfo.pVertexInputState = createInfo.m_VertexInputStates.data();
        pipelineInfo.pInputAssemblyState = &createInfo.m_InputAssemblyStateCreateInfo;
        pipelineInfo.pViewportState = &createInfo.m_ViewportState;
        pipelineInfo.pRasterizationState = &createInfo.m_Rasterizer;
        pipelineInfo.pMultisampleState = &createInfo.m_Multisampling;
        pipelineInfo.pDepthStencilState = createInfo.m_DepthStencilState; // Optional
        pipelineInfo.pColorBlendState = &createInfo.m_ColorBlendState;
        pipelineInfo.pDynamicState = createInfo.m_DynamicState; // Optional
        pipelineInfo.layout = m_PipelineLayout;
        pipelineInfo.renderPass = m_RenderPass->m_RenderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional

        if(!lDevice->CreateGraphicsPipeline(pipelineInfo, &m_Pipeline))
        {
            printf("Failed to create graphics pipeline!\n");

            for(VkPipelineShaderStageCreateInfo& stage : createInfo.m_ShaderStages)
            {
                vkDestroyShaderModule(lDevice->GetLogicalDevice(), stage.module, nullptr); 
            }

            return false;
        }

        delete createInfo.m_DynamicState;
        delete createInfo.m_DepthStencilState;

        for(VkPipelineShaderStageCreateInfo& stage : createInfo.m_ShaderStages)
        {
            vkDestroyShaderModule(lDevice->GetLogicalDevice(), stage.module, nullptr); 
        }

        return true;
    }

}}}