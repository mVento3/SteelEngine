#pragma once

#include "Core/Result.h"

#include "Vulkan/vulkan.h"

#include "Rendering/Vulkan/Shader.h"
#include "Rendering/Vulkan/VertexBuffer.h"
#include "Rendering/Vulkan/DescriptorSetLayout.h"

#include "Rendering/Vertex.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class SwapChain;
    class LogicalDevice;
    class RenderPass;
    class PhysicalDevice;

    class GraphicsPipeline
    {
        friend class DescriptorPool;
        friend class CommandPool;
    private:
        VkPipelineLayout    m_PipelineLayout;
        VkPipeline          m_GraphicsPipeline;

        std::vector<VkBuffer>       m_UniformBuffers;
        std::vector<VkDeviceMemory> m_UniformBuffersMemory;

    public:
        GraphicsPipeline();
        ~GraphicsPipeline();

        Result Create(
            const LogicalDevice& logicalDevice,
            const SwapChain& swapChain,
            const RenderPass& renderPass,
            const DescriptorSetLayout& desc,
            std::vector<VkPipelineShaderStageCreateInfo> stages
        );
        void Cleanup(const LogicalDevice& logicalDevice, const SwapChain& swapChain);

        Result CreateUniformBuffers(const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, const SwapChain& swapChain, const  Type::uint32 size);

        inline VkPipeline GetPipeline() { return m_GraphicsPipeline; }

        inline VkDeviceMemory GetUniformMemory(size_t index) { return m_UniformBuffersMemory[index]; }
    };

}}}