#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

#include "Core/Result.h"

namespace SteelEngine { namespace Graphics {

    struct IRenderer : public HotReload::IRuntimeObject
    {
        virtual Result Init() { return SE_NOT_IMPLEMENTED; }
        virtual void Update() override { }
        virtual void Cleanup() { }

        virtual void PreRender() { }
        virtual void Render() { }
        virtual void PostRender() { }
    };

}}