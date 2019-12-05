#pragma once

#include "Memory/Allocator.h"

#include "HotReloader/IRuntimeObject.h"

#include "Core/Result.h"

namespace SteelEngine {

    struct IMemoryTracker : public HotReloader::IRuntimeObject
    {
        virtual Result Init() { return SE_NOT_IMPLEMENTED; }
        virtual void Track(Memory::Allocator* allocator) = 0;
    };

}