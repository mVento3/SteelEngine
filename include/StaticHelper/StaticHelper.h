#pragma once

#include "RuntimeDatabase/RuntimeDatabase.h"

namespace SteelEngine {

    struct StaticHelper
    {
    protected:
        static RuntimeDatabase* ms_RuntimeDatabase;
    };

}