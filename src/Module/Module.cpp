#include "Module/Module.h"

#include "windows.h"

namespace SteelEngine { namespace Module {

    Result load(const char* filename, void** dll)
    {
        *dll = LoadLibraryA(filename); 

        if(*dll == NULL)
        {
            DWORD errorMessageID = GetLastError();

            LPSTR messageBuffer = nullptr;
            size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

            std::string message(messageBuffer, size);

            LocalFree(messageBuffer);

            printf("Module %s error: %s\n", filename, message.c_str());

            return Result(SE_FALSE, "Could not load library!");
        }

        return SE_TRUE;
    }

    // Result load(const std::string& filename, void** dll)
    // {
    //     *dll = LoadLibraryA(filename.c_str()); 

    //     if(!*dll)
    //     {
    //         DWORD errorMessageID = GetLastError();

    //         LPSTR messageBuffer = nullptr;
    //         size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    //                                     NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    //         std::string message(messageBuffer, size);

    //         LocalFree(messageBuffer);

    //         printf("Module %s error: %s\n", filename.c_str(), message.c_str());

    //         return Result(SE_FALSE, "Could not load library!");
    //     }

    //     return SE_TRUE;
    // }

    // Result load(const std::filesystem::path& filename, void** dll)
    // {
    //     return load(filename.string(), dll);
    // }

    Result get(const std::string& name, void* dll, void** result)
    {
        *result = (void*)GetProcAddress((HMODULE)dll, name.c_str());

        if(!*result)
        {
            return Result(SE_FALSE, "Could not get the variable!");
        }

        return SE_TRUE;
    }

    Result free(void** dll)
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