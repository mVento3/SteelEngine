#pragma once

#include "Window/IWindow.h"

#include "RuntimeCompiler/IRuntimeObject.h"

#include "Graphics/IRenderer.h"

namespace SteelEngine {

    struct IContext : public HotReload::IRuntimeObject
    {
        virtual void Init(IWindow* window, Graphics::IRenderer* renderer) { }
        virtual void UploadDrawData() { }
        virtual void ProcessEvent(const void* event) { }
    };

}