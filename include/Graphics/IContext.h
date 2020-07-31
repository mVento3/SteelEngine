#pragma once

#include "Window/IWindow.h"

#include "HotReloader/IRuntimeObject.h"

#include "Platform/Graphics/RenderDevice.h"

#include "functional"

namespace SteelEngine {

// Right now only for ImGUI
    struct IContext : public HotReloader::IRuntimeObject
    {
        void Empty(void*, uint32_t)
        {

        }

        void EmptyUpdate()
        {

        }

        virtual void Init(const IReflectionData* windowType, IWindow* window, const Graphics::RenderDevice* renderDevice) { }
        virtual void UploadDrawData() { }
        virtual void ProcessEvent(const void* event) { }

    // TODO: Refactor

        virtual std::function<void(void*, uint32_t)> GetEditorCommands() { return std::bind(&IContext::Empty, this, std::placeholders::_1, std::placeholders::_2); }
        virtual std::function<void()> GetUpdate() { return std::bind(&IContext::EmptyUpdate, this); }
    };

}