#pragma once

#include "ConsoleSystem/ICommand.h"

#include "RuntimeReflection/Macro.h"

#include "Core/ReflectionAttributes.h"

#include "ConsoleSystem/Commands/TestCommand2.Generated.h"

namespace SteelEngine {

    SE_CLASS(
        SteelEngine::ReflectionAttribute::CONSOLE_COMMAND
    )
    class TestCommand2 : public ICommand
    {
        GENERATED_BODY
    private:

    public:
        TestCommand2();
        ~TestCommand2();

        void Function(const std::vector<Options>& options) override;
    };

}