#pragma once

#include "Core/Result.h"

#include "Window/IWindow.h"

#include "HotReloader/IRuntimeObject.h"

#include "Graphics/IContext.h"
#include "Graphics/IRenderer.h"

namespace SteelEngine { namespace Editor {

    struct IEditor : public HotReloader::IRuntimeObject
    {
        virtual Result Init(Graphics::IRenderer* renderer, IContext* context) { return SE_NOT_IMPLEMENTED; }
        virtual void Draw() { }
        virtual void ProcessEvents(void* event) { }
    };

}}