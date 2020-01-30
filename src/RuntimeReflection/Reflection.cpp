#include "RuntimeReflection/Reflection.h"

#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {

    RuntimeDatabase* Reflection::ms_RuntimeDatabase;

    Reflection::Reflection()
    {

    }

    void Reflection::Init2()
    {
        SteelEngine::ReflectionRecorder::Register<Reflection>("Reflection",{
        "SteelEngine"
        }
        )
        (
        SteelEngine::Reflection::MetaData("sizeof", sizeof(Reflection)),
        SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true)
        )
        .Constructor<>()
        .Enum<Reflection::ReflectionAttribute>("ReflectionAttribute")
        .Values
        ({
        SteelEngine::ReflectionEnumElement("RUNTIME_SERIALIZE", Reflection::ReflectionAttribute::RUNTIME_SERIALIZE),
        SteelEngine::ReflectionEnumElement("UPDATE", Reflection::ReflectionAttribute::UPDATE),
        SteelEngine::ReflectionEnumElement("REFLECTION_MODULE", Reflection::ReflectionAttribute::REFLECTION_MODULE),
        SteelEngine::ReflectionEnumElement("NO_SERIALIZE", Reflection::ReflectionAttribute::NO_SERIALIZE),
        SteelEngine::ReflectionEnumElement("NET_VALUE", Reflection::ReflectionAttribute::NET_VALUE),
        SteelEngine::ReflectionEnumElement("NETWORK_COMMAND", Reflection::ReflectionAttribute::NETWORK_COMMAND),
        SteelEngine::ReflectionEnumElement("EDITOR", Reflection::ReflectionAttribute::EDITOR),
        SteelEngine::ReflectionEnumElement("EDITOR_NAME", Reflection::ReflectionAttribute::EDITOR_NAME),
        SteelEngine::ReflectionEnumElement("PROJECTS_PATH", Reflection::ReflectionAttribute::PROJECTS_PATH),
        SteelEngine::ReflectionEnumElement("VIRTUAL_PROJECT", Reflection::ReflectionAttribute::VIRTUAL_PROJECT),
        SteelEngine::ReflectionEnumElement("GAME_SCRIPT", Reflection::ReflectionAttribute::GAME_SCRIPT),
        SteelEngine::ReflectionEnumElement("ENGINE_START_TYPE", Reflection::ReflectionAttribute::ENGINE_START_TYPE),
        SteelEngine::ReflectionEnumElement("GENERATE_CAST_FUNCTIONS", Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS),
        SteelEngine::ReflectionEnumElement("DO_NOT_GENERATE_CAST_FUNCTIONS", Reflection::ReflectionAttribute::DO_NOT_GENERATE_CAST_FUNCTIONS),
        SteelEngine::ReflectionEnumElement("SYSTEMS_INFORMATION_TRACKER", Reflection::ReflectionAttribute::SYSTEMS_INFORMATION_TRACKER),
        SteelEngine::ReflectionEnumElement("NO_RECOMPILE", Reflection::ReflectionAttribute::NO_RECOMPILE),
        SteelEngine::ReflectionEnumElement("INHERITANCE_MODULE", Reflection::ReflectionAttribute::INHERITANCE_MODULE),
        SteelEngine::ReflectionEnumElement("NAIVE_EVENT", Reflection::ReflectionAttribute::NAIVE_EVENT),
        SteelEngine::ReflectionEnumElement("CONSOLE_COMMAND", Reflection::ReflectionAttribute::CONSOLE_COMMAND)
        })
        ;
    }

}