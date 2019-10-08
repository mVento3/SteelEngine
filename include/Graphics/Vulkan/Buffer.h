#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

#include "Graphics/Vulkan/IBuffer.h"

#include "Graphics/Vulkan/DeepLayer/IDevice.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
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
            const IDevice* device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkBuffer& buffer,
            VkDeviceMemory& bufferMemory
        );

        Result CreateBuffer(
            const IDevice* device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties
        ) override;
        Result CopyBuffer(
            const IDevice* device,
            VkBuffer srcBuffer,
            VkBuffer dstBuffer,
            VkDeviceSize size
        ) override;
        Result CopyBuffer(
            const IDevice* device,
            void* srcBuffer,
            VkDeviceSize size
        ) override;

        Result Map(
            const IDevice* device,
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkDeviceSize offset = 0
        ) override;
        Result Unmap(
            const IDevice* device
        ) override;
        Result Flush(
            const IDevice* device,
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkDeviceSize offset = 0
        ) override;

        void Cleanup(
            const IDevice* device
        ) override;

        inline const VkBuffer& GetBuffer() const override { return m_Buffer; }
        inline const Type::uint32 GetCount() const override { return m_Count; }
        inline const VkDeviceMemory& GetMemory() const override { return m_BufferMemory; }
        inline const void* GetMapped() const { return m_Mapped; }
    };

}}}