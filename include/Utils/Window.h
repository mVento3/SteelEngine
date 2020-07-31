#pragma once

#include "Window/IWindow.h"

namespace SteelEngine {

    class Window
    {
    public:
        enum WindowAPI
        {
            SDL,

            NONE
        };

    private:

    public:
        Window();
        ~Window();

        static IWindow* Create(WindowAPI api);
    };

}