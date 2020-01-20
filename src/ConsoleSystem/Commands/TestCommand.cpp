#include "ConsoleSystem/Commands/TestCommand.h"

namespace SteelEngine {

    TestCommand::TestCommand() :
        ICommand("TestCommand")
    {

    }

    TestCommand::~TestCommand()
    {

    }

    void TestCommand::Function(const std::vector<Options>& options)
    {
        printf("AAAAAAA\n");
    }

}