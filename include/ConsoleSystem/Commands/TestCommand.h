#pragma once

#include "ConsoleSystem/ICommand.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "ConsoleSystem/Commands/TestCommand.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::Reflection::ReflectionAttribute::CONSOLE_COMMAND,
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class TestCommand : public ICommand
    {
        GENERATED_BODY
    private:

    public:
        TestCommand();
        ~TestCommand();

        void Function(const std::vector<Options>& options) override;
    };

}