#include "Rendering/Vulkan/VertexBuffer.h"

#include "Rendering/Vulkan/Renderer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    Result VertexBuffer::SetupMemory(Renderer* renderer)
    {
        vkGetBufferMemoryRequirements(renderer->m_LogicalDevice->GetLogicalDevice(), m_VertexBuffer, &m_MemoryRequirements);

        VkMemoryAllocateInfo allocInfo = {};

        allocInfo.sType =           VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize =  m_MemoryRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(renderer->m_PhysicalDevice, m_MemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if(vkAllocateMemory(renderer->m_LogicalDevice->GetLogicalDevice(), &allocInfo, nullptr, &m_VertexBufferMemory) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        vkBindBufferMemory(renderer->m_LogicalDevice->GetLogicalDevice(), m_VertexBuffer, m_VertexBufferMemory, 0);

        return SE_TRUE;
    }

    VertexBuffer::VertexBuffer()
    {

    }

    VertexBuffer::~VertexBuffer()
    {

    }

    Type::uint32 VertexBuffer::FindMemoryType(PhysicalDevice* physicalDevice, Type::uint32 typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;

        vkGetPhysicalDeviceMemoryProperties(physicalDevice->m_PhysicalDevice, &memProperties);

        for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        throw std::runtime_error("Failed to find suitable memory type!");
    }

    void VertexBuffer::Cleanup(Renderer* renderer)
    {
        vkDestroyBuffer(renderer->m_LogicalDevice->GetLogicalDevice(), m_VertexBuffer, nullptr);
    }

}}}