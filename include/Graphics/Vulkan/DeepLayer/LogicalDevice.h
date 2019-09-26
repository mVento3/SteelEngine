#pragma once

#include "Vulkan/vulkan.h"

#include "Core/Result.h"

#include "Graphics/Vulkan/DeepLayer/QueueFamilyIndices.h"

#include "set"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    class PhysicalDevice;
    class SwapChain;
    class Surface;

    class LogicalDevice
    {
        friend class PhysicalDevice;
        friend class Renderer;
        friend class Buffer;

        friend class Device;
    private:
        VkDevice m_LogicalDevice;

        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;

    public:
        LogicalDevice();
        ~LogicalDevice();

        Result Create(const PhysicalDevice& physicalDevice, const Surface& surface);

        inline VkDevice GetLogicalDevice() const { return m_LogicalDevice; }
    };

}}}