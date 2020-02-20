#pragma once

#include "RuntimeReflection/MetaDataImplementation.h"

#include "string"

namespace SteelEngine {

    struct IReflectionInheritance : public MetaDataImplementation
    {
        virtual const std::string& GetName() const = 0;
        virtual size_t GetTypeID() const = 0;
    };

}