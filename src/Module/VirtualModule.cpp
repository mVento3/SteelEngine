#include "Module/VirtualModule.h"

#include "MemoryModule/MemoryModule.h"

#include "Windows.h"

namespace SteelEngine { namespace VirtualModule {

    void* readLibrary(const char* filename, size_t* size)
    {
        size_t read;
        void* result;
        FILE* file;

        file = fopen(filename, "rb");

        if(!file)
        {
            printf("Cannot open %s\n", filename);

            return 0;
        }

        fseek(file, 0, SEEK_END);

        *size = static_cast<size_t>(ftell(file));

        if(*size == 0)
        {
            fclose(file);

            return 0;
        }

        result = (unsigned char*)malloc(*size);

        if(!result)
        {
            fclose(file);

            return 0;
        }

        fseek(file, 0, SEEK_SET);

        read = fread(result, 1, *size, file);

        fclose(file);

        if(read != *size)
        {
            free(result);

            return 0;
        }

        return result;
    }

    Result memoryLoad(const std::string& filename, void** dll, void** memory)
    {
        size_t size;
        void* data = readLibrary(filename.c_str(), &size);

        if(!data)
        {
            return SE_FALSE;
        }

        HMEMORYMODULE handle = MemoryLoadLibrary(data, size);

        if(!handle)
        {
            printf("Cannot load library from memory!\n");

            free(data);

            return SE_FALSE;
        }

        *dll = handle;
        *memory = data;

        return SE_TRUE;
    }

    Result memoryGet(const std::string& name, void* dll, void** result)
    {
        *result = (void*)MemoryGetProcAddress(dll, name.c_str());

        if(!*result)
        {
            printf("Could not get the variable!");

            return SE_FALSE;
        }

        return SE_TRUE;
    }

    Result memoryFree(void** dll, void** memory)
    {
        MemoryFreeLibrary(*dll);

        free(*memory);

        *memory = 0;

        return SE_TRUE;
    }

}}