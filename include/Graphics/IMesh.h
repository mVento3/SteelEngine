#pragma once

#include "Core/Type.h"

namespace SteelEngine { namespace Graphics {

    class RenderDevice;

    struct IMesh
    {
    // TODO: Temp
        virtual void Setup(const RenderDevice* renderDevice) { }

        virtual void** GetVAO_() const { return 0; }
        virtual void** GetVBOs_() const { return 0; }
        virtual void SetDrawCount(uint32_t drawCount) { }
        virtual uint32_t GetDrawCount() const { return 0; }
    };

}}