#pragma once

#include "Graphics/ITexture.h"

namespace SteelEngine { namespace Graphics {

    struct IMaterial
    {
        virtual ITexture* Setup() = 0;
    };

}}