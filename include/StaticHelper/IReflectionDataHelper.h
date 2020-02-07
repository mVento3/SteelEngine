#pragma once

#include "vector"

namespace SteelEngine {

    namespace HotReloader {

    struct IRuntimeObject;

    }

    struct IReflectionData;
    struct IReflectionInheritance;

    struct IReflectionDataHelper
    {
        void ProcessInheritance(const std::vector<IReflectionData*>& res, const std::vector<IReflectionInheritance*>& inheritance, HotReloader::IRuntimeObject* createdObject, const IReflectionData* data) const;
    };
        
}