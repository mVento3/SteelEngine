#pragma once

#include "Vulkan/vulkan.h"

#include "optional"
#include "vector"

#include "Rendering/Vulkan/PhysicalDevice.h"
#include "Rendering/Vulkan/Surface.h"

#include "Core/Type.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    struct QueueFamilyIndices
    {
        std::optional<Type::uint32> m_GraphicsFamily;
        std::optional<Type::uint32> m_PresentFamily;

        bool IsComplete()
        {
            return m_GraphicsFamily.has_value() && m_PresentFamily.has_value();
        }

        static QueueFamilyIndices FindQueueFamilies(PhysicalDevice* device, Surface* surface)
        {
            return FindQueueFamilies(device->m_PhysicalDevice, surface->m_Surface);
        }

        static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
        {
            QueueFamilyIndices indices;

            uint32_t queueFamilyCount = 0;

            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            int i = 0;

            for(const auto& queueFamily : queueFamilies)
            {
                if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    indices.m_GraphicsFamily = i;
                }

                VkBool32 presentSupport = false;

                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

                if(queueFamily.queueCount > 0 && presentSupport)
                {
                    indices.m_PresentFamily = i;
                }

                if(indices.IsComplete())
                {
                    break;
                }

                i++;
            }

            return indices;
        }
    };

}}}