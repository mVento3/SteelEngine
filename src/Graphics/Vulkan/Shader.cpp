#include "Graphics/Vulkan/Shader.h"

#include "fstream"

#include "FileSystem/FileSystem.h"

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

    VkShaderModule Shader::CreateShaderModule(
        const LogicalDevice& logicalDevice,
        const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo = {};

        createInfo.sType =      VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize =   code.size();
        createInfo.pCode =      reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;

        if(vkCreateShaderModule(logicalDevice.GetLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create shader module!");
        }

        return shaderModule;
    }

    Shader::Shader()
    {

    }

    Shader::~Shader()
    {

    }

    Result Shader::LoadShader(
        const LogicalDevice& logicalDevice,
        const std::string& name,
        std::vector<VkPipelineShaderStageCreateInfo>& stages)
    {
        m_Filename = name;

        auto vertShaderCode = ReadFile((FileSystem::Get("shaders") / name).string() + ".vs");
        auto fragShaderCode = ReadFile((FileSystem::Get("shaders") / name).string() + ".fs");

        VkShaderModule vertShaderModule = CreateShaderModule(logicalDevice, vertShaderCode);
        VkShaderModule fragShaderModule = CreateShaderModule(logicalDevice, fragShaderCode);

        m_Modules.push_back(vertShaderModule);
        m_Modules.push_back(fragShaderModule);

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
        
        stages.push_back(vertShaderStageInfo);
        stages.push_back(fragShaderStageInfo);
        
        return SE_TRUE;
    }

    Result Shader::LoadShader(
        const LogicalDevice& logicalDevice,
        std::vector<VkPipelineShaderStageCreateInfo>& stages)
    {
        return LoadShader(logicalDevice, m_Filename, stages);
    }

    void Shader::Destroy(const LogicalDevice& logicalDevice)
    {
        vkDestroyShaderModule(logicalDevice.GetLogicalDevice(), m_Modules[1], nullptr);
        vkDestroyShaderModule(logicalDevice.GetLogicalDevice(), m_Modules[0], nullptr);

        m_Modules.clear();
    }

}}}