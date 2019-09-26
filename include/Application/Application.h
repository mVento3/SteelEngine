#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

namespace SteelEngine {

    class Application : public HotReload::IRuntimeObject
    {
    private:

    public:
        Application();
        ~Application();
    };

}