#pragma once

#include "Core/Result.h"

#include "Window/IWindow.h"

#include "HotReloader/IRuntimeObject.h"
#include "Graphics/IContext.h"
#include "Graphics/ITexture.h"

#include "Utils/Graphics/RenderContext.h"

#include "VirtualProject/IVirtualProjectVisualizer.h"

#include "functional"

namespace SteelEngine { namespace Editor {

    struct IEditor : public HotReloader::IRuntimeObject
    {
        virtual Result Init(Utils::RenderContext* renderContext, IWindow* window) { return SE_NOT_IMPLEMENTED; }
        virtual void Draw(void* finalTexture) { }
        virtual void ProcessEvents(void* event) { }

        virtual void Render(IVirtualProjectVisualizer* visualizer) { visualizer->Render(); }
        virtual void Init(IVirtualProjectVisualizer* visualizer, IContext* context) { visualizer->Init(context); }

        virtual void SetVirtualProjectVisualizer(IVirtualProjectVisualizer** visualizer) { }

    // TODO: Maybe change that?

        virtual std::function<void(void*, uint32_t)> GetEditorCommands() = 0;
        virtual std::function<void()> GetUpdate() = 0;
    };

}}