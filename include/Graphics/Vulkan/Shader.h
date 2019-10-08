#pragma once

#include "string"
#include "vector"

#include "Core/Type.h"
#include "Core/Result.h"
#include "Core/ReflectionAttributes.h"

#include "Vulkan/vulkan.h"

#include "Graphics/Vulkan/IShader.h"

#include "RuntimeReflection/Macro.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    class Shader : public IShader
    {
    private:
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
        ) override;
        // Result LoadShader(
        //     const LogicalDevice& logicalDevice,
        //     std::vector<VkPipelineShaderStageCreateInfo>& stages
        // ) override;
        void Destroy(const LogicalDevice& logicalDevice) override;
    };

}}}