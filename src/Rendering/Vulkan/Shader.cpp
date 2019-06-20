#include "Rendering/Vulkan/Shader.h"

#include "fstream"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    std::vector<char> Shader::ReadFile(const std::string& filename)
    {
        std::ifstream file(filename.c_str(), std::ios::ate | std::ios::binary);

        if(!file.is_open())
        {
            throw std::runtime_error("Failed to open file!");
        }

        Type::uint32 fileSize = (Type::uint32)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    VkShaderModule Shader::CreateShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo = {};

        createInfo.sType =      VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize =   code.size();
        createInfo.pCode =      reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;

        if(vkCreateShaderModule(m_LogicalDevice->GetLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create shader module!");
        }

        return shaderModule;
    }

    Shader::Shader(LogicalDevice* logicalDevice) :
        m_LogicalDevice(logicalDevice)
    {

    }

    Shader::~Shader()
    {

    }

    Result Shader::LoadShader(
        const std::string& name,
        std::vector<VkPipelineShaderStageCreateInfo>& stages,
        std::vector<VkShaderModule>& modules)
    {
        auto vertShaderCode = ReadFile("bin/Resources/Shaders/vert.spv");
        auto fragShaderCode = ReadFile("bin/Resources/Shaders/frag.spv");

        VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

        modules.push_back(vertShaderModule);
        modules.push_back(fragShaderModule);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};

        vertShaderStageInfo.sType =     VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage =     VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module =    vertShaderModule;
        vertShaderStageInfo.pName =     "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};

        fragShaderStageInfo.sType =     VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage =     VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module =    fragShaderModule;
        fragShaderStageInfo.pName =     "main";

        // VkPipelineShaderStageCreateInfo shaderStages[] =
        //     {
        //         vertShaderStageInfo,
        //         fragShaderStageInfo
        //     };
        
        stages.push_back(vertShaderStageInfo);
        stages.push_back(fragShaderStageInfo);
        
        return SE_TRUE;
    }

}}}