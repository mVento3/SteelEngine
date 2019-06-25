#pragma once

#include "Core/Result.h"
#include "Core/Type.h"

#include "Vulkan/vulkan.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Renderer;
    class PhysicalDevice;
    class LogicalDevice;

    class VertexBuffer
    {
    private:
        VkBuffer                m_VertexBuffer;
        VkMemoryRequirements    m_MemoryRequirements;
        VkDeviceMemory          m_VertexBufferMemory;
        Type::uint32            m_Size;

        Result SetupMemory(Renderer* renderer);

    public:
        VertexBuffer();
        ~VertexBuffer();

        static Type::uint32 FindMemoryType(PhysicalDevice* physicalDevice, Type::uint32 typeFilter, VkMemoryPropertyFlags properties);

        template <typename A>
        Result CreateVertexBuffer(Renderer* renderer, Type::uint32 size, A* data);
        void Cleanup(Renderer* renderer);

        inline Type::uint32 GetSize() { return m_Size; }
        inline VkBuffer GetBuffer() { return m_VertexBuffer; }
    };

    template <typename A>
    Result VertexBuffer::CreateVertexBuffer(Renderer* renderer, Type::uint32 size, A* data_)
    {
        m_Size = size;

        VkBufferCreateInfo bufferInfo = {};

        bufferInfo.sType =          VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size =           size;
        bufferInfo.usage =          VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode =    VK_SHARING_MODE_EXCLUSIVE;

        if(vkCreateBuffer(renderer->m_LogicalDevice->GetLogicalDevice(), &bufferInfo, nullptr, &m_VertexBuffer) != VK_SUCCESS)
        {
            return SE_FALSE;
        }

        if(SetupMemory(renderer) == SE_FALSE)
        {
            return SE_FALSE;
        }

        void* data;

        vkMapMemory(renderer->m_LogicalDevice->GetLogicalDevice(), m_VertexBufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, data_, (size_t)bufferInfo.size);
        vkUnmapMemory(renderer->m_LogicalDevice->GetLogicalDevice(), m_VertexBufferMemory);

        return SE_TRUE;
    }

}}}