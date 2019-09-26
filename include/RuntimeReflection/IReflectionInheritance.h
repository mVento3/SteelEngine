#pragma once

namespace SteelEngine {

    struct IReflectionInheritance
    {
        virtual const std::string& GetName() = 0;
        virtual size_t GetTypeID() = 0;
    };

}