#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

#include "Graphics/Vulkan/DeepLayer/IDevice.h"

#include "vector"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class DescriptorPool
    {
    private:
        VkDescriptorPool                m_DescriptorPool;
        std::vector<VkDescriptorSet>    m_DescriptorSets;

        uint32_t m_Size;
        std::vector<VkWriteDescriptorSet> m_DescriptorWrites;
        uint32_t m_WriteIndex;

    public:
        DescriptorPool();
        ~DescriptorPool();

        Result Create(
            const IDevice* device,
            uint32_t descriptorCount,
            uint32_t maxSets
        );
        Result AllocateDescriptorSets(
            const IDevice* device,
            const VkDescriptorSetLayout& descriptorSetLayout
        );
        void WriteDescriptorSet(
            const std::vector<VkDescriptorBufferInfo>& buffers
        );
        void UpdateDescriptorSets(const IDevice* device);

        inline const VkDescriptorSet& GetDescriptorSet(uint32_t index) const { return m_DescriptorSets[index]; }
    };

}}}