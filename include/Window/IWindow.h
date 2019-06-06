#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

#include "Core/Result.h"

namespace SteelEngine { namespace Interface {

    struct IWindow : public IRuntimeObject
    {
        struct WindowCloseEvent
        {

        };

        virtual Result Create() { return SE_NOT_IMPLEMENTED; }
        virtual void Close() { }

        virtual void SetTitle(const std::string& title) { }
        virtual void SetWidth(const Type::uint32& width) { }
        virtual void SetHeight(const Type::uint32& height) { }
    };

}}