#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/Nodes/EntryPoint.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/ImGUI_Editor/EditorWindows/NodeGraph/Nodes/EntryPoint.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<EntryPoint>("EntryPoint",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(EntryPoint))
)
.RegisterConstructor<>()
.RegisterInheritance<NodeGraph::INode>("NodeGraph::INode")
.RegisterMethod("Func", { SteelEngine::Reflection::FucntionArgument<NodeGraph::INode*>("node"),
SteelEngine::Reflection::FucntionArgument<VisualScript::IPinData*>("input") }, &EntryPoint::Func)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FLOW, true)
)
;
}
}
