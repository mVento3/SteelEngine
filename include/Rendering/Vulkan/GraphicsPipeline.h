#pragma once

#include "Core/Result.h"

#include "Vulkan/vulkan.h"

#include "Rendering/Vulkan/Shader.h"
#include "Rendering/Vulkan/VertexBuffer.h"

#include "Rendering/Vertex.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class SwapChain;
    class LogicalDevice;
    class RenderPass;

    class GraphicsPipeline
    {
    private:
        VkPipelineLayout    m_PipelineLayout;
        VkPipeline          m_GraphicsPipeline;

    public:
        GraphicsPipeline();
        ~GraphicsPipeline();

        Result Create(const LogicalDevice& logicalDevice, const SwapChain& swapChain, const RenderPass& renderPass, std::vector<VkPipelineShaderStageCreateInfo> stages);
        void Cleanup(const LogicalDevice& logicalDevice);

        inline VkPipeline GetPipeline() { return m_GraphicsPipeline; }
    };

}}}