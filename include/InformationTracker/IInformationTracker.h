#pragma once

#include "InformationTracker/ISystemTracker.h"

#include "HotReloader/IRuntimeObject.h"

namespace SteelEngine {

    struct IInformationTracker : public HotReloader::IRuntimeObject
    {
        virtual void Add(ISystemTracker* tracker) = 0;
        virtual ISystemTracker* Get(size_t typeID) = 0;
    };

}