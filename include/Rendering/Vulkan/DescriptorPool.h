#pragma once

#include "Core/Result.h"

#include "Vulkan/vulkan.h"

#include "vector"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class SwapChain;
    class LogicalDevice;
    class DescriptorSetLayout;
    class GraphicsPipeline;

    class DescriptorPool
    {
        friend class CommandPool;
    private:
        VkDescriptorPool m_DescriptorPool;

        std::vector<VkDescriptorSet> m_DescriptorSets;

        Result CreateDescriptorSets(
            const LogicalDevice& logicalDevice,
            const SwapChain& swapChain,
            const DescriptorSetLayout& descriptorSetLayout,
            const GraphicsPipeline& graphicsPipeline
        );

    public:
        DescriptorPool();
        ~DescriptorPool();

        Result Create(
            const LogicalDevice& logicalDevice,
            const SwapChain& swapChain,
            const DescriptorSetLayout& descriptorSetLayout,
            const GraphicsPipeline& graphicsPipeline
        );
        void Cleanup(const LogicalDevice& logicalDevice);
    };

}}}