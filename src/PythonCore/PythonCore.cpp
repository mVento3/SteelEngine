#include "PythonCore/PythonCore.h"

namespace SteelEngine { namespace Script {

    PythonCore::PythonCore()
    {
        
    }

    PythonCore::~PythonCore()
    {

    }

    Result PythonCore::Init()
    {
        Py_Initialize();

        return SE_TRUE;
    }

    void PythonCore::Finalize()
    {
        Py_Finalize();
    }

}}