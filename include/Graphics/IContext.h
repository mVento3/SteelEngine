#pragma once

#include "Window/IWindow.h"

#include "HotReloader/IRuntimeObject.h"

#include "Graphics/IRenderer.h"

namespace SteelEngine {

    struct IContext : public HotReloader::IRuntimeObject
    {
        virtual void Init(IWindow* window, Graphics::IRenderer* renderer) { }
        virtual void UploadDrawData() { }
        virtual void ProcessEvent(const void* event) { }
    };

}