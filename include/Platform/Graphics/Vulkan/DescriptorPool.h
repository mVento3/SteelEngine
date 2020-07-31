#pragma once

#include "Platform/Graphics/Vulkan/LogicalDevice.h"
#include "Platform/Graphics/Vulkan/Texture.h"

#include "array"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    class CommandBuffer;
    class Pipeline;

    class DescriptorPool
    {
        friend class CommandBuffer;
        friend class Pipeline;
    private:
        VkDescriptorPool m_DescriptorPool;
        std::vector<VkDescriptorSet> m_DescriptorSets;

    public:
        DescriptorPool();
        ~DescriptorPool();

        bool SetupDescriptorPool(LogicalDevice* lDevice, const Pipeline* pipeline, uint32_t poolSize, uint32_t maxSets = 3);
        bool SetupDescriptorSets(LogicalDevice* lDevice, const Pipeline* pipeline, uint32_t poolSize, Texture* texture);
        bool SetupDescriptorSet(LogicalDevice* lDevice, const Pipeline* pipeline, const VkDescriptorImageInfo& info, VkDescriptorSet& descriptorSet);
    };

}}}