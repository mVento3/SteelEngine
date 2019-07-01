#include "Rendering/Vulkan/DescriptorPool.h"

#include "Rendering/Vulkan/SwapChain.h"
#include "Rendering/Vulkan/LogicalDevice.h"
#include "Rendering/Vulkan/DescriptorSetLayout.h"
#include "Rendering/Vulkan/GraphicsPipeline.h"

#include "Rendering/UniformBufferObject.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    Result DescriptorPool::CreateDescriptorSets(
        const LogicalDevice& logicalDevice,
        const SwapChain& swapChain,
        const DescriptorSetLayout& descriptorSetLayout,
        const GraphicsPipeline& graphicsPipeline)
    {
        std::vector<VkDescriptorSetLayout> layouts(swapChain.m_SwapChainImages.size(), descriptorSetLayout.m_DescriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo = {};

        allocInfo.sType =               VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool =      m_DescriptorPool;
        allocInfo.descriptorSetCount =  swapChain.m_SwapChainImages.size();
        allocInfo.pSetLayouts =         layouts.data();

        m_DescriptorSets.resize(swapChain.m_SwapChainImages.size());

        if(vkAllocateDescriptorSets(logicalDevice.GetLogicalDevice(), &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        for(size_t i = 0; i < swapChain.m_SwapChainImages.size(); i++)
        {
            VkDescriptorBufferInfo bufferInfo = {};

            bufferInfo.buffer = graphicsPipeline.m_UniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range =  sizeof(UniformBufferObject);

            VkWriteDescriptorSet descriptorWrite = {};

            descriptorWrite.sType =             VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet =            m_DescriptorSets[i];
            descriptorWrite.dstBinding =        0;
            descriptorWrite.dstArrayElement =   0;
            descriptorWrite.descriptorType =    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount =   1;
            descriptorWrite.pBufferInfo =       &bufferInfo;
            descriptorWrite.pImageInfo =        nullptr; // Optional
            descriptorWrite.pTexelBufferView =  nullptr; // Optional

            vkUpdateDescriptorSets(logicalDevice.GetLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
        }

        return SE_TRUE;
    }

    DescriptorPool::DescriptorPool()
    {

    }

    DescriptorPool::~DescriptorPool()
    {

    }

    Result DescriptorPool::Create(
        const LogicalDevice& logicalDevice,
        const SwapChain& swapChain,
        const DescriptorSetLayout& descriptorSetLayout,
        const GraphicsPipeline& graphicsPipeline)
    {
        VkDescriptorPoolSize poolSize = {};

        poolSize.type =             VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount =  swapChain.m_SwapChainImages.size();

        VkDescriptorPoolCreateInfo poolInfo = {};

        poolInfo.sType =            VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount =    1;
        poolInfo.pPoolSizes =       &poolSize;
        poolInfo.maxSets =          swapChain.m_SwapChainImages.size();

        if(vkCreateDescriptorPool(logicalDevice.GetLogicalDevice(), &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        if(CreateDescriptorSets(logicalDevice, swapChain, descriptorSetLayout, graphicsPipeline) == SE_FALSE)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    void DescriptorPool::Cleanup(const LogicalDevice& logicalDevice)
    {
        vkDestroyDescriptorPool(logicalDevice.GetLogicalDevice(), m_DescriptorPool, nullptr);
    }

}}}