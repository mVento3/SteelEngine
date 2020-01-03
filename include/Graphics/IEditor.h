#pragma once

#include "Core/Result.h"

#include "Window/IWindow.h"

#include "HotReloader/IRuntimeObject.h"

#include "Graphics/IContext.h"
#include "Graphics/IRenderer.h"

#include "VirtualProject/IVirtualProjectVisualizer.h"

namespace SteelEngine { namespace Editor {

    struct IEditor : public HotReloader::IRuntimeObject
    {
        virtual Result Init(Graphics::IRenderer* renderer, IContext* context) { return SE_NOT_IMPLEMENTED; }
        virtual void Draw() { }
        virtual void ProcessEvents(void* event) { }

        virtual void Render(IVirtualProjectVisualizer* visualizer) { visualizer->Render(); }
        virtual void Init(IVirtualProjectVisualizer* visualizer, IContext* context) { visualizer->Init(context); }

        virtual void SetVirtualProjectVisualizer(IVirtualProjectVisualizer** visualizer) { }
    };

}}