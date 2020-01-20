#include "ConsoleSystem/Commands/TestCommand2.h"

namespace SteelEngine {

    TestCommand2::TestCommand2() :
        ICommand("TestCommand2")
    {

    }

    TestCommand2::~TestCommand2()
    {

    }

    void TestCommand2::Function(const std::vector<Options>& options)
    {
        printf("BBBBBBB");
    }

}