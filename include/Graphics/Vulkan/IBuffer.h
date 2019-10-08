#pragma once

#include "Core/Type.h"
#include "Core/Result.h"

#include "Vulkan/vulkan.h"

#include "Graphics/Vulkan/DeepLayer/IDevice.h"

#include "RuntimeCompiler/IRuntimeObject.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    struct IBuffer : public HotReload::IRuntimeObject
    {
        virtual Result CreateBuffer(
            const IDevice* device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties
        ) = 0;
        virtual Result CopyBuffer(
            const IDevice* device,
            VkBuffer srcBuffer,
            VkBuffer dstBuffer,
            VkDeviceSize size
        ) = 0;
        virtual Result CopyBuffer(
            const IDevice* device,
            void* srcBuffer,
            VkDeviceSize size
        ) = 0;

        virtual Result Map(
            const IDevice* device,
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkDeviceSize offset = 0
        ) = 0;
        virtual Result Unmap(
            const IDevice* device
        ) = 0;
        virtual Result Flush(
            const IDevice* device,
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkDeviceSize offset = 0
        ) = 0;

        virtual void Cleanup(
            const IDevice* device
        ) = 0;

        virtual const VkBuffer& GetBuffer() const = 0;
        virtual const Type::uint32 GetCount() const = 0;
        virtual const VkDeviceMemory& GetMemory() const = 0;
    };

}}}