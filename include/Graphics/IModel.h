#pragma once

#include "Graphics/IMesh.h"

namespace SteelEngine { namespace Graphics {

    struct IModel
    {
        virtual IMesh* Setup() = 0;
    };

}}