#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "Graphics/IContext.h"

namespace SteelEngine {

    namespace Editor {

    struct IEditor;

    }

    struct IVirtualProjectVisualizer : public HotReloader::IRuntimeObject
    {
        friend struct Editor::IEditor;
    protected:
        virtual void Render() { }
        virtual void Init(IContext* context) { }

    public:
        virtual void OnProjectLoad() { }
    };

}