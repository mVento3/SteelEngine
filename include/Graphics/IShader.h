#pragma once

#include "Platform/Graphics/RenderDevice.h"

namespace SteelEngine { namespace Graphics {

    struct IShader
    {
        virtual void Setup(const RenderDevice* renderDevice) = 0;

        virtual void** GetProgram() const = 0;
        virtual void* GetLocation(uint32_t index) const = 0;
    };

}}