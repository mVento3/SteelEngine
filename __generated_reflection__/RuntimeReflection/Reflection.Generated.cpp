#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "RuntimeReflection/Reflection.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/RuntimeReflection/Reflection.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Reflection>("Reflection",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Reflection))
)
.RegisterInheritance<StaticHelper>("StaticHelper")
.RegisterEnum<Reflection::ReflectionAttribute>("ReflectionAttribute")
.Values({
SteelEngine::Reflection::EnumElement("RUNTIME_SERIALIZE", Reflection::ReflectionAttribute::RUNTIME_SERIALIZE),
SteelEngine::Reflection::EnumElement("UPDATE", Reflection::ReflectionAttribute::UPDATE),
SteelEngine::Reflection::EnumElement("REFLECTION_MODULE", Reflection::ReflectionAttribute::REFLECTION_MODULE),
SteelEngine::Reflection::EnumElement("NO_SERIALIZE", Reflection::ReflectionAttribute::NO_SERIALIZE),
SteelEngine::Reflection::EnumElement("NET_VALUE", Reflection::ReflectionAttribute::NET_VALUE),
SteelEngine::Reflection::EnumElement("NETWORK_COMMAND", Reflection::ReflectionAttribute::NETWORK_COMMAND),
SteelEngine::Reflection::EnumElement("EDITOR", Reflection::ReflectionAttribute::EDITOR),
SteelEngine::Reflection::EnumElement("EDITOR_NAME", Reflection::ReflectionAttribute::EDITOR_NAME),
SteelEngine::Reflection::EnumElement("PROJECTS_PATH", Reflection::ReflectionAttribute::PROJECTS_PATH),
SteelEngine::Reflection::EnumElement("VIRTUAL_PROJECT", Reflection::ReflectionAttribute::VIRTUAL_PROJECT),
SteelEngine::Reflection::EnumElement("GAME_SCRIPT", Reflection::ReflectionAttribute::GAME_SCRIPT),
SteelEngine::Reflection::EnumElement("ENGINE_START_TYPE", Reflection::ReflectionAttribute::ENGINE_START_TYPE),
SteelEngine::Reflection::EnumElement("GENERATE_CAST_FUNCTIONS", Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS),
SteelEngine::Reflection::EnumElement("DO_NOT_GENERATE_CAST_FUNCTIONS", Reflection::ReflectionAttribute::DO_NOT_GENERATE_CAST_FUNCTIONS),
SteelEngine::Reflection::EnumElement("SYSTEMS_INFORMATION_TRACKER", Reflection::ReflectionAttribute::SYSTEMS_INFORMATION_TRACKER),
SteelEngine::Reflection::EnumElement("NO_RECOMPILE", Reflection::ReflectionAttribute::NO_RECOMPILE),
SteelEngine::Reflection::EnumElement("INHERITANCE_MODULE", Reflection::ReflectionAttribute::INHERITANCE_MODULE),
SteelEngine::Reflection::EnumElement("NAIVE_EVENT", Reflection::ReflectionAttribute::NAIVE_EVENT),
SteelEngine::Reflection::EnumElement("CONSOLE_COMMAND", Reflection::ReflectionAttribute::CONSOLE_COMMAND),
SteelEngine::Reflection::EnumElement("HOT_RELOAD", Reflection::ReflectionAttribute::HOT_RELOAD),
SteelEngine::Reflection::EnumElement("GENERATE_OWN_SERIALIZE_FUNC", Reflection::ReflectionAttribute::GENERATE_OWN_SERIALIZE_FUNC),
SteelEngine::Reflection::EnumElement("CAST_FUNCTION", Reflection::ReflectionAttribute::CAST_FUNCTION),
SteelEngine::Reflection::EnumElement("SERIALIZE_FUNCTION", Reflection::ReflectionAttribute::SERIALIZE_FUNCTION),
SteelEngine::Reflection::EnumElement("VISUAL_SCRIPT_NODE", Reflection::ReflectionAttribute::VISUAL_SCRIPT_NODE),
SteelEngine::Reflection::EnumElement("VISUAL_SCRIPT_FUNC", Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC),
SteelEngine::Reflection::EnumElement("VISUAL_SCRIPT_INPUT", Reflection::ReflectionAttribute::VISUAL_SCRIPT_INPUT),
SteelEngine::Reflection::EnumElement("VISUAL_SCRIPT_OUTPUT", Reflection::ReflectionAttribute::VISUAL_SCRIPT_OUTPUT),
SteelEngine::Reflection::EnumElement("VISUAL_SCRIPT_FLOW", Reflection::ReflectionAttribute::VISUAL_SCRIPT_FLOW),
SteelEngine::Reflection::EnumElement("VISUAL_SCRIPT_RETURN_TYPE", Reflection::ReflectionAttribute::VISUAL_SCRIPT_RETURN_TYPE)
})
;
}
}
