#pragma once

#include "vector"

namespace SteelEngine {

    struct IReflectionData;
    struct IReflectionInheritance;

    struct IReflectionDataHelper
    {
        void ProcessInheritance(const std::vector<IReflectionData*>& res, const std::vector<IReflectionInheritance*>& inheritance, void* createdObject, const IReflectionData* data) const;
        bool Process(const IReflectionData* data) const;
    };
        
}