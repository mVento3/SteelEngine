#pragma once

#include "Platform/Graphics/Vulkan/Buffer.h"

namespace SteelEngine { namespace Platform { namespace Vulkan {

    class VertexBuffer : public Buffer
    {
    private:

    public:
        VertexBuffer(PhysicalDevice* pDevice, LogicalDevice* lDevice);
        ~VertexBuffer();

        bool CreateVertexBuffer(
            size_t size,
            const void* data,
            uint32_t verticesCount
        );
    };

}}}