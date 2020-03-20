#pragma once

#include "string"

namespace SteelEngine {

    struct IFunctionArgument
    {
        virtual size_t GetTypeID() const = 0;
        virtual const std::string& GetName() const = 0;
    };

}