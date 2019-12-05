#pragma once

#include "HotReloader/IRuntimeObject.h"

namespace SteelEngine {

    class Application : public HotReloader::IRuntimeObject
    {
    private:

    public:
        Application();
        ~Application();
    };

}