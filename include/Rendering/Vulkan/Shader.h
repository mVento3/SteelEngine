#pragma once

#include "string"
#include "vector"

#include "Core/Type.h"
#include "Core/Result.h"

#include "Vulkan/vulkan.h"

#include "Rendering/Vulkan/LogicalDevice.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Shader
    {
    private:
        static std::vector<char> ReadFile(const std::string& filename);

        VkShaderModule CreateShaderModule(const std::vector<char>& code);

        LogicalDevice* m_LogicalDevice;

    public:
        Shader(LogicalDevice* logicalDevice);
        ~Shader();

        Result LoadShader(
            const std::string& name,
            std::vector<VkPipelineShaderStageCreateInfo>& stages,
            std::vector<VkShaderModule>& modules
        );
    };

}}}