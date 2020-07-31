#include "Platform/Graphics/Vulkan/Texture.h"

#include "Platform/Graphics/Vulkan/Vulkan.h"
#include "Platform/Graphics/Vulkan/LogicalDevice.h"

#define STB_IMAGE_IMPLEMENTATION

namespace SteelEngine { namespace Platform { namespace Vulkan {

    bool Texture::CreateTextureImage(uint32_t width, uint32_t height)
    {
        VkImageCreateInfo image = {};
        image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image.imageType = VK_IMAGE_TYPE_2D;
		image.format = m_Format;
		image.extent.width = width;
		image.extent.height = height;
		image.extent.depth = 1;
		image.mipLevels = 1;
		image.arrayLayers = 1;
		image.samples = VK_SAMPLE_COUNT_1_BIT;
		image.tiling = VK_IMAGE_TILING_OPTIMAL;
		image.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

        m_LDevice->CreateImage(image, &m_TextureImage);

        VkMemoryRequirements memoryRequirements;

        if(!m_LDevice->GetImageMemoryRequirements(m_TextureImage, &memoryRequirements))
        {
            return false;
        }

        VkMemoryAllocateInfo memoryAllocateInfo = {};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex =
            m_PDevice->FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if(!m_LDevice->AllocateMemory(memoryAllocateInfo, &m_TextureImageMemory) ||
            !m_LDevice->BindImageMemory(m_TextureImage, m_TextureImageMemory))
        {
            return false;
        }

        return true;
    }

    bool Texture::CreateTextureImage(const void* pixels, uint32_t width, uint32_t height)
    {
        VkDeviceSize imageSize = width * height * 4;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        SteelEngine::Graphics::createBuffer(m_PDevice, m_LDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        m_LDevice->MapMemory(stagingBufferMemory, imageSize, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        m_LDevice->UnmapMemory(stagingBufferMemory);

        if(!SteelEngine::Graphics::createImage(
            m_PDevice,
            m_LDevice,
            width,
            height,
            m_Format,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_TextureImage,
            m_TextureImageMemory))
        {
            m_LDevice->DestroyBuffer(stagingBuffer);
            m_LDevice->FreeMemory(stagingBufferMemory);

            return false;
        }

        if(!SteelEngine::Graphics::transitionImageLayout(m_LDevice, m_CommandPool, m_GraphicsQueue, m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL))
        {
            m_LDevice->DestroyBuffer(stagingBuffer);
            m_LDevice->FreeMemory(stagingBufferMemory);

            return false;
        }

        SteelEngine::Graphics::copyBufferToImage(m_LDevice, m_CommandPool, m_GraphicsQueue, stagingBuffer, m_TextureImage, width, height);

        if(!SteelEngine::Graphics::transitionImageLayout(m_LDevice, m_CommandPool, m_GraphicsQueue, m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL))
        {
            m_LDevice->DestroyBuffer(stagingBuffer);
            m_LDevice->FreeMemory(stagingBufferMemory);

            return false;
        }

        m_LDevice->DestroyBuffer(stagingBuffer);
        m_LDevice->FreeMemory(stagingBufferMemory);

        return true;
    }

    bool Texture::CreateTextureImageView()
    {
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_TextureImage;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = m_Format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        m_LDevice->CreateImageView(viewInfo, &m_TextureImageView);

        return true;
    }

    bool Texture::CreateTextureSampler()
    {
        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if(!m_LDevice->CreateSampler(samplerInfo, &m_TextureSampler))
        {
            printf("Failed to create texture sampler!\n");

            return false;
        }

        return true;
    }

    Texture::Texture(PhysicalDevice* pDevice, LogicalDevice* lDevice, VkCommandPool commandPool, VkQueue gQueue) :
        m_PDevice(pDevice),
        m_LDevice(lDevice),
        m_CommandPool(commandPool),
        m_GraphicsQueue(gQueue)
    {

    }

    Texture::~Texture()
    {

    }

    bool Texture::Setup(uint32_t width, uint32_t height, const VkFormat& format)
    {
        m_Format = format;

        if(!CreateTextureImage(width, height))
        {
            printf("Setting texture image up failed!\n");

            return false;
        }

        if(!CreateTextureImageView())
        {
            printf("Setting texture image view up failed!\n");

            return false;
        }

        if(!CreateTextureSampler())
        {
            printf("Setting texture sampler up failed!\n");

            return false;
        }

        return true;
    }

    bool Texture::Setup(const void* pixels, uint32_t width, uint32_t height, const VkFormat& format)
    {
        m_Format = format;

        if(!CreateTextureImage(pixels, width, height))
        {
            printf("Setting texture image up failed!\n");

            return false;
        }

        if(!CreateTextureImageView())
        {
            printf("Setting texture image view up failed!\n");

            return false;
        }

        if(!CreateTextureSampler())
        {
            printf("Setting texture sampler up failed!\n");

            return false;
        }

        return true;
    }

    bool Texture::Setup(const void* pixels, uint32_t width, uint32_t height, SteelEngine::Graphics::ITexture* texture, const VkFormat& formate)
    {
        if(!Setup(pixels, width, height))
        {
            return false;
        }

        *texture->GetTextureID_Ptr() = this;

        return true;
    }

    VkDescriptorImageInfo Texture::GetDescriptorImageInfo()
    {
        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = m_TextureImageView;
        imageInfo.sampler = m_TextureSampler;

        return imageInfo;
    }

    VkDescriptorImageInfo Texture::GetDescriptorImageInfo() const
    {
        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = m_TextureImageView;
        imageInfo.sampler = m_TextureSampler;

        return imageInfo;
    }

}}}