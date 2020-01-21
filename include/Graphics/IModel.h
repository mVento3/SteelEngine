#pragma once

#include "Graphics/IMesh.h"

#include "HotReloader/IRuntimeObject.h"

namespace SteelEngine { namespace Graphics {

    struct IModel : public HotReloader::IRuntimeObject
    {
        virtual IMesh* Setup() = 0;
    };

}}