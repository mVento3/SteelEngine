#pragma once

#include "HotReloader/IRuntimeObject.h"

#include "ConsoleSystem/ICommand.h"

namespace SteelEngine {

    struct IConsoleSystem : public HotReloader::IRuntimeObject
    {
        virtual bool Init() = 0;
        virtual void RegisterCommand(ICommand* command) = 0;
        virtual std::vector<ICommand*> GetCommands(const char* name) = 0;
        virtual ICommand* GetCommand(const char* name) = 0;
    };

}