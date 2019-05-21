#include "Module/Module.h"

#include "windows.h"

namespace SteelEngine { namespace Module {

    Result Load(const std::string& filename, void** dll)
    {
        *dll = LoadLibraryA(filename.c_str());

        if(!*dll)
        {
            return Result(SE_FALSE, "Could not load library!");
        }

        return SE_TRUE;
    }

    Result Get(const std::string& name, void* dll, void** result)
    {
        *result = (void*)GetProcAddress((HMODULE)dll, name.c_str());

        if(!*result)
        {
            return Result(SE_FALSE, "Could not get the variable!");
        }

        return SE_TRUE;
    }

    Result Free(void** dll)
    {
        if(FreeLibrary((HMODULE)*dll) == 0)
        {
            DWORD error = GetLastError();
            LPTSTR errorText = NULL;

			FormatMessage(
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                0,
                error,
                MAKELANGID(
                    LANG_NEUTRAL,
                    SUBLANG_DEFAULT
                ),
                (LPTSTR)&errorText,
                0,
                0
            );

			return Result(SE_FALSE, (const char*)errorText);
        }

        *dll = 0;
        dll = 0;

        return SE_TRUE;
    }

}}