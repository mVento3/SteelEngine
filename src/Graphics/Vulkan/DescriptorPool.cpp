#include "Graphics/Vulkan/DescriptorPool.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    DescriptorPool::DescriptorPool()
    {
        m_WriteIndex = 0;
    }

    DescriptorPool::~DescriptorPool()
    {

    }

    Result DescriptorPool::Create(
        const IDevice* device,
        uint32_t descriptorCount,
        uint32_t maxSets)
    {
        m_Size = descriptorCount;

        VkDescriptorPoolSize poolSize = {};

        poolSize.type =             VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount =  descriptorCount;

        VkDescriptorPoolCreateInfo descriptorPoolInfo = {};

        descriptorPoolInfo.sType =            VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.poolSizeCount =    1;
        descriptorPoolInfo.pPoolSizes =       &poolSize;
        descriptorPoolInfo.maxSets =          maxSets;

        if(device->CreateDescriptorPool(
            &descriptorPoolInfo,
            m_DescriptorPool) == SE_FALSE)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    Result DescriptorPool::AllocateDescriptorSets(
        const IDevice* device,
        const VkDescriptorSetLayout& descriptorSetLayout)
    {
        std::vector<VkDescriptorSetLayout> layouts(m_Size, descriptorSetLayout);
        VkDescriptorSetAllocateInfo descriptorAllocInfo = {};

        descriptorAllocInfo.sType =               VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorAllocInfo.descriptorPool =      m_DescriptorPool;
        descriptorAllocInfo.descriptorSetCount =  layouts.size();
        descriptorAllocInfo.pSetLayouts =         layouts.data();

        m_DescriptorSets.resize(layouts.size());

        if(device->AllocateDescriptorSets(
            &descriptorAllocInfo,
            m_DescriptorSets) == SE_FALSE)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    void DescriptorPool::WriteDescriptorSet(
        const std::vector<VkDescriptorBufferInfo>& buffers)
    {
        VkWriteDescriptorSet descriptorWrite = {};

        descriptorWrite.sType =             VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet =            m_DescriptorSets[m_WriteIndex];
        descriptorWrite.dstBinding =        0;
        descriptorWrite.dstArrayElement =   0;
        descriptorWrite.descriptorType =    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount =   buffers.size();
        descriptorWrite.pBufferInfo =       buffers.data();
        descriptorWrite.pImageInfo =        nullptr; // Optional
        descriptorWrite.pTexelBufferView =  nullptr; // Optional

        m_DescriptorWrites.push_back(descriptorWrite);
        m_WriteIndex++;
    }

    void DescriptorPool::UpdateDescriptorSets(const IDevice* device)
    {
        device->UpdateDescriptorSets(m_DescriptorWrites);
    }

}}}