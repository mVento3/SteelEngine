#pragma once

#include "string"

namespace SteelEngine {

    struct ISystemTracker
    {
        virtual void Track(const std::string& type, const std::string& info) = 0;
        virtual size_t GetTypeID() = 0;
    };

}