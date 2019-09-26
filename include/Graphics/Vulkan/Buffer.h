#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

#include "Graphics/IBuffer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class Device;

    class Buffer : public IBuffer
    {
    protected:
        VkBuffer        m_Buffer;
        VkDeviceMemory  m_BufferMemory;
        Type::uint32    m_Count;
        void*           m_Mapped;

    public:
        Buffer();
        ~Buffer();

        static Result CreateBuffer(
            const Device* device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkBuffer& buffer,
            VkDeviceMemory& bufferMemory
        );

        Result CreateBuffer(
            const Device* device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties
        );
        Result CopyBuffer(
            const Device* device,
            VkBuffer srcBuffer,
            VkBuffer dstBuffer,
            VkDeviceSize size
        );

        template <typename A>
        Result CopyBuffer(
            const Device* device,
            A srcBuffer,
            VkDeviceSize size
        );

        Result Map(
            const Device* device,
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkDeviceSize offset = 0
        );
        Result Unmap(
            const Device* device
        );
        Result Flush(
            const Device* device,
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkDeviceSize offset = 0
        );

        virtual void Cleanup(
            const Device* device
        );

        inline const VkBuffer GetBuffer() { return m_Buffer; }
        inline const Type::uint32 GetCount() { return m_Count; }
        inline const VkDeviceMemory GetMemory() { return m_BufferMemory; }

        template <typename A>
        inline A GetMapped() { return (A)m_Mapped; }
    };

    template <typename A>
    Result Buffer::CopyBuffer(
        const Device* device,
        A srcBuffer,
        VkDeviceSize size)
    {
        device->MapMemory(
            m_BufferMemory,
            VK_WHOLE_SIZE,
            &m_Mapped
        );

        memcpy(m_Mapped, srcBuffer, size);

        device->UnmapMemory(
            m_BufferMemory
        );

        return SE_TRUE;
    }

}}}