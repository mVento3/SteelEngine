#pragma once

#include "Core/Result.h"

#include "Graphics/Vulkan/DeepLayer/LogicalDevice.h"

#include "HotReloader/IRuntimeObject.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    struct IShader : public HotReloader::IRuntimeObject
    {
        virtual Result LoadShader(
            const LogicalDevice& logicalDevice,
            const std::string& name,
            std::vector<VkPipelineShaderStageCreateInfo>& stages
        ) = 0;
        // virtual Result LoadShader(
        //     const LogicalDevice& logicalDevice,
        //     std::vector<VkPipelineShaderStageCreateInfo>& stages
        // ) = 0;
        virtual void Destroy(const LogicalDevice& logicalDevice) = 0;
    };
    

}}}