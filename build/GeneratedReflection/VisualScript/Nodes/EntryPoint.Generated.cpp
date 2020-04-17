#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "VisualScript/Nodes/EntryPoint.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/VisualScript/Nodes/EntryPoint.Generated.h"

namespace SteelEngine {
namespace VisualScript {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<EntryPoint>("EntryPoint",{
"SteelEngine",
"VisualScript"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(EntryPoint))
)
.RegisterConstructor<>()
.RegisterInheritance<NodeTemplate>("NodeTemplate")
.RegisterMethod("Func", { SteelEngine::Reflection::FucntionArgument<VisualScript::INodeData*>("node"),
SteelEngine::Reflection::FucntionArgument<VisualScript::IPinData*>("input") }, &EntryPoint::Func)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FLOW, true)
)
;
}
}
}
