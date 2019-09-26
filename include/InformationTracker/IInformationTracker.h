#pragma once

#include "InformationTracker/ISystemTracker.h"

#include "RuntimeCompiler/IRuntimeObject.h"

namespace SteelEngine {

    struct IInformationTracker : public HotReload::IRuntimeObject
    {
        virtual void Add(ISystemTracker* tracker) = 0;
        virtual ISystemTracker* Get(size_t typeID) = 0;
    };

}