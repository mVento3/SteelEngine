#pragma once

#include "Core/Result.h"

#include "Window/IWindow.h"

#include "RuntimeCompiler/IRuntimeObject.h"

// #include "Graphics/Vulkan/IProgramUserData.h"
#include "ImGUI_Editor/IContext.h"

#include "Graphics/IRenderer.h"

namespace SteelEngine { namespace Editor {

    struct IEditor : public HotReload::IRuntimeObject
    {
        virtual Result Init(Graphics::IRenderer* renderer, IContext* context) { return SE_NOT_IMPLEMENTED; }
        virtual void Draw() { }
        virtual void ProcessEvents(void* event) { }
    };

}}