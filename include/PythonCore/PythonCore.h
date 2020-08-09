#pragma once

#include "PythonCore/IPythonCore.h"
#include "PythonCore/IScript.h"

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "PythonCore/PythonCore.Generated.h"

#ifdef _DEBUG
    #undef _DEBUG
    #include "Python.h"
    #define _DEBUG
#else
    #include "Python.h"
#endif

#include "vector"

namespace SteelEngine { namespace Script {

    SE_CLASS(
        Reflection::ReflectionAttribute::HOT_RELOAD
    )
    class PythonCore : public IPython
    {
        GENERATED_BODY
    private:
        std::vector<Python::IScript*> m_Scripts;

    public:
        PythonCore();
        ~PythonCore();

        Result Init() override;
        void Finalize() override;
    };

}}