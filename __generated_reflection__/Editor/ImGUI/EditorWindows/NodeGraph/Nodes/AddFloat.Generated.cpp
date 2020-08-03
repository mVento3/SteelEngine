#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Editor/ImGUI/EditorWindows/NodeGraph/Nodes/AddFloat.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Editor/ImGUI/EditorWindows/NodeGraph/Nodes/AddFloat.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<AddFloat>("AddFloat",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(AddFloat))
)
.RegisterConstructor<>()
.RegisterInheritance<NodeGraph::INode>("NodeGraph::INode")
.RegisterMethod("Func", { SteelEngine::Reflection::FucntionArgument<NodeGraph::INode*>("node"),
SteelEngine::Reflection::FucntionArgument<VisualScript::IPinData*>("input") }, &AddFloat::Func)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_RETURN_TYPE , typeid(float).hash_code())
)
.RegisterMethod("Func2", { SteelEngine::Reflection::FucntionArgument<NodeGraph::INode*>("node"),
SteelEngine::Reflection::FucntionArgument<VisualScript::IPinData*>("input") }, &AddFloat::Func2)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FLOW, true)
)
;
}
}
