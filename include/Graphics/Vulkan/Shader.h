#pragma once

#include "string"
#include "vector"

#include "Core/Type.h"
#include "Core/Result.h"

#include "Vulkan/vulkan.h"

#include "Graphics/Vulkan/DeepLayer/LogicalDevice.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Shader
    {
    private:
        std::string m_Filename;

        std::vector<VkShaderModule> m_Modules;

        static std::vector<char> ReadFile(const std::string& filename);

        VkShaderModule CreateShaderModule(
            const LogicalDevice& logicalDevice,
            const std::vector<char>& code
        );

    public:
        Shader();
        ~Shader();

        Result LoadShader(
            const LogicalDevice& logicalDevice,
            const std::string& name,
            std::vector<VkPipelineShaderStageCreateInfo>& stages
        );
        Result LoadShader(
            const LogicalDevice& logicalDevice,
            std::vector<VkPipelineShaderStageCreateInfo>& stages
        );
        void Destroy(const LogicalDevice& logicalDevice);
    };

}}}