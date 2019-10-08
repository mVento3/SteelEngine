#pragma once

#include "Core/Result.h"
#include "Core/Type.h"

#include "Graphics/Vulkan/Buffer.h"

#include "Vulkan/vulkan.h"

namespace SteelEngine { namespace Graphics { namespace Vulkan {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::NO_SERIALIZE
    )
    struct VertexBuffer : public Buffer
    {
    private:

    public:
        VertexBuffer();
        ~VertexBuffer();

        Result CreateVertexBuffer(
            const IDevice* device,
            size_t size,
            const void* data,
            Type::uint32 verticesCount
        );
    };

}}}