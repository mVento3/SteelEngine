#pragma once

#include "vulkan/vulkan.h"

#include "Graphics/ITexture.h"

#include "Platform/Graphics/ITexture.h"

#include "Platform/Graphics/Vulkan/LogicalDevice.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    class Texture : public SteelEngine::Platform::Graphics::ITexture
    {
    private:
        VkImage m_TextureImage;
        VkDeviceMemory m_TextureImageMemory;
        VkImageView m_TextureImageView;
        VkSampler m_TextureSampler;

        LogicalDevice* m_LDevice;
        PhysicalDevice* m_PDevice;
        VkCommandPool m_CommandPool;
        VkQueue m_GraphicsQueue;

        VkFormat m_Format;

        bool CreateTextureImage(uint32_t width, uint32_t height);
        bool CreateTextureImage(const void* pixels, uint32_t width, uint32_t height);
        bool CreateTextureImageView();
        bool CreateTextureSampler();

    public:
        Texture(PhysicalDevice* pDevice, LogicalDevice* lDevice, VkCommandPool commandPool, VkQueue gQueue);
        ~Texture();

        bool Setup(uint32_t width, uint32_t height, const VkFormat& format);
        bool Setup(const void* pixels, uint32_t width, uint32_t height, const VkFormat& format = VK_FORMAT_R8G8B8A8_SRGB);
        bool Setup(const void* pixels, uint32_t width, uint32_t height, SteelEngine::Graphics::ITexture* texture, const VkFormat& format = VK_FORMAT_R8G8B8A8_SRGB);

        VkDescriptorImageInfo GetDescriptorImageInfo();
        VkDescriptorImageInfo GetDescriptorImageInfo() const;
    };

}}}