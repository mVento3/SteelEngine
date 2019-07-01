#pragma once

#include "Core/Result.h"

#include "Vulkan/vulkan.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class LogicalDevice;

    class DescriptorSetLayout
    {
        friend class GraphicsPipeline;
        friend class DescriptorPool;
    private:
        VkDescriptorSetLayout m_DescriptorSetLayout;

    public:
        DescriptorSetLayout();
        ~DescriptorSetLayout();

        Result Create(const LogicalDevice& logicalDevice);
        void Cleanup(const LogicalDevice& logicalDevice);
    };

}}}