#pragma once

#include "RuntimeCompiler/IRuntimeObject.h"

namespace SteelEngine { namespace Interface {

    struct ICore : public IRuntimeObject
    {
        virtual void Start() { }
        virtual void Stop() { }
    };

}}