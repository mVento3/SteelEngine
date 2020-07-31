#pragma once

#include "Platform/Graphics/Vulkan/Buffer.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    class IndexBuffer : public Buffer
    {
    private:

    public:
        IndexBuffer(PhysicalDevice* pDevice, LogicalDevice* lDevice);
        ~IndexBuffer();

        bool CreateIndexBuffer(
            size_t size,
            const void* data,
            uint32_t indicesCount
        );
    };

}}}