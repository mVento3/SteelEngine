#pragma once

#include "vulkan/vulkan.h"

#include "Platform/Graphics/Vulkan/LogicalDevice.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    class Buffer
    {
    protected:
        VkBuffer        m_Buffer;
        VkDeviceMemory  m_BufferMemory;
        uint32_t        m_Count;
        void*           m_Mapped;

        PhysicalDevice* m_PDevice;
        LogicalDevice* m_LDevice;
        VkCommandPool m_CommandPool;
        VkQueue m_GraphicsQueue;

    public:
        Buffer(PhysicalDevice* pDevice, LogicalDevice* lDevice);
        ~Buffer();

        static bool CreateBuffer(
            PhysicalDevice* pDevice,
            LogicalDevice* lDevice,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkBuffer& buffer,
            VkDeviceMemory& bufferMemory
        );

        bool CreateBuffer(
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties
        );
        bool CopyBuffer(
            VkBuffer srcBuffer,
            VkBuffer dstBuffer,
            VkDeviceSize size
        );
        bool CopyBuffer(
            void* srcBuffer,
            VkDeviceSize size
        );

        bool Map(
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkDeviceSize offset = 0
        );
        bool Unmap();
        bool Flush(
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkDeviceSize offset = 0
        );

        void Cleanup();

        inline const VkBuffer& GetBuffer() const
        {
            return m_Buffer;
        }

        inline const uint32_t GetCount() const
        {
            return m_Count;
        }

        inline const VkDeviceMemory& GetMemory() const
        {
            return m_BufferMemory;
        }

        inline const void* GetMapped() const
        {
            return m_Mapped;
        }
    };

}}}