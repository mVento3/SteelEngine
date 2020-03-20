#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "VisualScript/Nodes/GeneralFunctions.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/VisualScript/Nodes/GeneralFunctions.Generated.h"

namespace SteelEngine {
namespace VisualScript {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<GeneralFunctions>("GeneralFunctions",{
"SteelEngine",
"VisualScript"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_NODE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(GeneralFunctions))
)
.Constructor<>()
.Method("EntryPoint", { SteelEngine::Reflection::FucntionArgument<VisualScript::INodeData*>("node"),
SteelEngine::Reflection::FucntionArgument<VisualScript::IPinData*>("input") }, &GeneralFunctions::EntryPoint)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_OUTPUT, true)
)
.Method("Add", { SteelEngine::Reflection::FucntionArgument<VisualScript::INodeData*>("node"),
SteelEngine::Reflection::FucntionArgument<VisualScript::IPinData*>("input") }, &GeneralFunctions::Add)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_OUTPUT, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_INPUT, true)
)
;
}
}
}
