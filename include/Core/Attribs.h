#pragma once

#include "RuntimeReflection/Macro.h"

#include "RuntimeCompiler/IRuntimeObject.h"

#include "Attribs.Generated.h"

namespace SteelEngine {

    SE_CLASS()
    class Attribs : public Interface::IRuntimeObject
    {
        GENERATED_BODY
    public:
        SE_ENUM()
        enum TestE
        {
            SE_ELEMENT()
            TEST1,
            SE_ELEMENT()
            TEST2
        };

        SE_ENUM()
        enum TestE2
        {
            SE_ELEMENT()
            TESTE1,
            SE_ELEMENT()
            TESTE2
        };

    private:

    public:
        Attribs();
        ~Attribs();
    };

}