#pragma once

#include "Utils/Json.h"

namespace SteelEngine {

    struct JsonSerialization
    {
        virtual void Serialize(Utils::json& j) = 0;
        virtual void Deserialize(const Utils::json& j) = 0;
    };

}