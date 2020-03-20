#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "VisualScript/Nodes/AddFloat.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/VisualScript/Nodes/AddFloat.Generated.h"

namespace SteelEngine {
namespace VisualScript {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<AddFloat>("AddFloat",{
"SteelEngine",
"VisualScript"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(AddFloat))
)
.Constructor<>()
.Inheritance<NodeTemplate>("NodeTemplate")
.Method("Func", { SteelEngine::Reflection::FucntionArgument<VisualScript::INodeData*>("node"),
SteelEngine::Reflection::FucntionArgument<VisualScript::IPinData*>("input") }, &AddFloat::Func)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_RETURN_TYPE , typeid(float).hash_code())
)
.Method("Func2", { SteelEngine::Reflection::FucntionArgument<VisualScript::INodeData*>("node"),
SteelEngine::Reflection::FucntionArgument<VisualScript::IPinData*>("input") }, &AddFloat::Func2)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FLOW, true)
)
;
}
}
}
