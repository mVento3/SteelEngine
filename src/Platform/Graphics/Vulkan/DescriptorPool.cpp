#include "Platform/Graphics/Vulkan/DescriptorPool.h"

#include "Platform/Graphics/Vulkan/Pipeline.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    DescriptorPool::DescriptorPool()
    {

    }

    DescriptorPool::~DescriptorPool()
    {

    }

    bool DescriptorPool::SetupDescriptorPool(LogicalDevice* lDevice, const Pipeline* pipeline, uint32_t poolSize, uint32_t maxSets)
    {
        std::vector<VkDescriptorPoolSize> poolSizes;

        for(uint32_t i = 0; i < pipeline->m_CreateInfo.m_DescriptorSetLayoutBindings.size(); i++)
        {
            VkDescriptorPoolSize poolSize_ = {};
            poolSize_.type = pipeline->m_CreateInfo.m_DescriptorSetLayoutBindings[i].descriptorType;
            poolSize_.descriptorCount = poolSize;

            poolSizes.push_back(poolSize_);
        }

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = maxSets;

        if(!lDevice->CreateDescriptorPool(poolInfo, &m_DescriptorPool))
        {
            printf("Failed to create descriptor pool!\n");

            return false;
        }

        // std::vector<VkDescriptorSetLayout> layouts(createInfo.m_PoolSize, createInfo.m_Pipeline->m_DescriptorSetLayout);
        // VkDescriptorSetAllocateInfo allocInfo = {};
        // allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        // allocInfo.descriptorPool = m_DescriptorPool;
        // allocInfo.descriptorSetCount = static_cast<uint32_t>(createInfo.m_PoolSize);
        // allocInfo.pSetLayouts = layouts.data();

        // m_DescriptorSets.resize(createInfo.m_PoolSize);

        // if(!lDevice->AllocateDescriptorSets(allocInfo, m_DescriptorSets.data()))
        // {
        //     printf("Failed to allocate descriptor sets!\n");

        //     return false;
        // }

        // for(size_t i = 0; i < createInfo.m_PoolSize; i++)
        // {
        //     VkDescriptorImageInfo imageInfo = createInfo.m_Texture->GetDescriptorImageInfo();

        //     VkWriteDescriptorSet descriptorWrite = {};
        //     descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        //     descriptorWrite.dstSet = m_DescriptorSets[i];
        //     descriptorWrite.dstBinding = 0; // TODO: remeber about that!!!!!!!
        //     descriptorWrite.dstArrayElement = 0;
        //     descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //     descriptorWrite.descriptorCount = 1;
        //     descriptorWrite.pImageInfo = &imageInfo;

        //     lDevice->UpdateDescriptorSet(descriptorWrite);
        // }

        return true;
    }

    bool DescriptorPool::SetupDescriptorSets(LogicalDevice* lDevice, const Pipeline* pipeline, uint32_t poolSize, Texture* texture)
    {
        std::vector<VkDescriptorSetLayout> layouts(poolSize, pipeline->m_DescriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_DescriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(poolSize);
        allocInfo.pSetLayouts = layouts.data();

        m_DescriptorSets.resize(poolSize);

        if(!lDevice->AllocateDescriptorSets(allocInfo, m_DescriptorSets.data()))
        {
            printf("Failed to allocate descriptor sets!\n");

            return false;
        }

        for(size_t i = 0; i < poolSize; i++)
        {
            VkDescriptorImageInfo imageInfo = texture->GetDescriptorImageInfo();

            VkWriteDescriptorSet descriptorWrite = {};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = m_DescriptorSets[i];
            descriptorWrite.dstBinding = 0; // TODO: remeber about that!!!!!!!
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pImageInfo = &imageInfo;

            lDevice->UpdateDescriptorSet(descriptorWrite);
        }

        return true;
    }

    bool DescriptorPool::SetupDescriptorSet(LogicalDevice* lDevice, const Pipeline* pipeline, const VkDescriptorImageInfo& info, VkDescriptorSet& descriptorSet)
    {
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType =               VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool =      m_DescriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &pipeline->m_DescriptorSetLayout;

        if(!lDevice->AllocateDescriptorSets(allocInfo, &descriptorSet))
        {
            printf("Failed to allocate descriptor sets!\n");

            return false;
        }

        VkWriteDescriptorSet writeDescriptorSet = {};

        writeDescriptorSet.sType =              VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.dstSet =             descriptorSet;
        writeDescriptorSet.descriptorType =     VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeDescriptorSet.dstBinding =         0;
        writeDescriptorSet.pImageInfo =         &info;
        writeDescriptorSet.descriptorCount =    1;

        lDevice->UpdateDescriptorSet(writeDescriptorSet);

        return true;
    }

}}}