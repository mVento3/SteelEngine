#pragma once

#include "ConsoleSystem/IConsoleSystem.h"
#include "ConsoleSystem/ICommand.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "ConsoleSystem/ConsoleSystem.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class ConsoleSystem : public IConsoleSystem
    {
        GENERATED_BODY
    private:
        std::vector<ICommand*> m_Commands;

    public:
        ConsoleSystem();
        ~ConsoleSystem();

        bool Init() override;
        void RegisterCommand(ICommand* command) override;
        std::vector<ICommand*> GetCommands(const char* name) override;
        ICommand* GetCommand(const char* name) override;
    };

}