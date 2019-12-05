#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "Core/Result.h"

namespace SteelEngine { namespace Graphics {

    struct IRenderer : public HotReloader::IRuntimeObject
    {
        enum API
        {
            OPENGL_API,
            VULKAN_API,
            SELECTED_RENDER_API
        };

    // General functions
        virtual Result Init() { return SE_NOT_IMPLEMENTED; }
        virtual void Update() override { }
        virtual void Cleanup() { }

        virtual void PreRender() { }
        virtual void Render() { }
        virtual void PostRender() { }
    };

}}