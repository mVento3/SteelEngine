#pragma once

#include "vulkan/vulkan.h"

#include "vector"
#include "optional"
#include "set"
#include "string"

namespace SteelEngine {

    namespace Graphics {

        class Vulkan;

    }

    namespace Platform { namespace Vulkan {

    class Surface;

    class PhysicalDevice
    {
        friend class SteelEngine::Graphics::Vulkan;
    public:
        struct QueueFamilyIndices
        {
            std::optional<uint32_t> m_GraphicsFamily;
            std::optional<uint32_t> m_PresentFamily;

            bool IsComplete()
            {
                return m_GraphicsFamily.has_value() &&
                    m_PresentFamily.has_value();
            }
        };

        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR        m_Capabilities;
            std::vector<VkSurfaceFormatKHR> m_Formats;
            std::vector<VkPresentModeKHR>   m_PresentModes;
        };

    private:
        VkPhysicalDevice m_PhysicalDevice;

        bool IsDeviceSuitable(const VkPhysicalDevice& pDevice, Surface* surface, const std::vector<const char*>& extensions);
        QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& pDevice, Surface* surface);
        bool CheckDeviceExtensionSupport(const VkPhysicalDevice& pDevice, const std::vector<const char*>& extensions);
        SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& pDevice, Surface* surface);

    public:
        PhysicalDevice();
        ~PhysicalDevice();

        QueueFamilyIndices FindQueueFamilies(Surface* surface);
        SwapChainSupportDetails QuerySwapChainSupport(Surface* surface);
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void GetPhysicalDeviceFormatProperties(const VkFormat& format, VkFormatProperties* formatProperties);
        bool GetSupportedDepthFormat(VkFormat* depthFormat);

        bool PickPhysicalDevice(const VkInstance& instance, Surface* surface, const std::vector<const char*>& extensions);

        bool CreateLogicalDevice(const VkDeviceCreateInfo& createInfo, VkDevice* lDevice);

    // TODO: Temp
        inline VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
    };

}}}