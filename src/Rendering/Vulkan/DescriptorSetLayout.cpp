#include "Rendering/Vulkan/DescriptorSetLayout.h"

#include "Rendering/Vulkan/LogicalDevice.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    DescriptorSetLayout::DescriptorSetLayout()
    {

    }

    DescriptorSetLayout::~DescriptorSetLayout()
    {

    }

    Result DescriptorSetLayout::Create(const LogicalDevice& logicalDevice)
    {
        std::vector<VkDescriptorSetLayoutBinding> uboLayoutBindings;

        VkDescriptorSetLayoutBinding uboLayoutBinding = {};

        uboLayoutBinding.binding =              0;
        uboLayoutBinding.descriptorType =       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount =      1;
        uboLayoutBinding.stageFlags =           VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers =   nullptr; // Optional

        uboLayoutBindings.push_back(uboLayoutBinding);

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};

        layoutInfo.sType =          VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount =   uboLayoutBindings.size();
        layoutInfo.pBindings =      uboLayoutBindings.data();

        if(vkCreateDescriptorSetLayout(logicalDevice.GetLogicalDevice(), &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        return SE_TRUE;
    }

    void DescriptorSetLayout::Cleanup(const LogicalDevice& logicalDevice)
    {
        vkDestroyDescriptorSetLayout(logicalDevice.GetLogicalDevice(), m_DescriptorSetLayout, nullptr);
    }

}}}