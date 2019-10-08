#pragma once

#include "Graphics/Vulkan/Buffer.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    class IndexBuffer : public Buffer
    {
    private:

    public:
        IndexBuffer();
        ~IndexBuffer();

        Result CreateIndexBuffer(
            const IDevice* device,
            size_t size,
            const void* data,
            Type::uint32 indicesCount
        );
    };

}}}