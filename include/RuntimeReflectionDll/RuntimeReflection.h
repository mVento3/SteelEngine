#pragma once

#include "Serialization/byte_buffer.h"

#include "PlatformDll/containers.h"

#include "RuntimeReflection/Macro.h"

SE_DONT_PARSE

namespace SteelEngine {

    se_declare_resource_type(command_buffer);
    se_declare_resource_type(reflection_type);

    struct reflection_type_info
    {
        size_t typeID;
        char name[30];
    };

    struct RuntimeReflectionDOD
    {
        void* data;

        bool (*init)(RuntimeReflectionDOD*);

        void (*reset_command_buffer)(void*, se_resource(command_buffer));
        void (*submit_command_buffer)(void*, se_resource(command_buffer));

        se_resource(command_buffer) (*construct_command_buffer)(void*);
        se_resource(reflection_type) (*construct_reflection_type)(void*, const reflection_type_info&);
    };

    typedef RuntimeReflectionDOD* (*runtime_reflection_construct_callback)();

}

extern "C" __declspec(dllexport) SteelEngine::RuntimeReflectionDOD* runtime_reflection_construct();