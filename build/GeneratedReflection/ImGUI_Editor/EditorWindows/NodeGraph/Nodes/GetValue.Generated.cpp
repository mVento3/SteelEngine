#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/Nodes/GetValue.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/ImGUI_Editor/EditorWindows/NodeGraph/Nodes/GetValue.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<GetValue>("GetValue",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(GetValue))
)
.RegisterConstructor<>()
.RegisterInheritance<NodeGraph::INode>("NodeGraph::INode")
.RegisterMethod("GetFloat", { SteelEngine::Reflection::FucntionArgument<NodeGraph::INode*>("node"),
SteelEngine::Reflection::FucntionArgument<VisualScript::IPinData*>("input") }, &GetValue::GetFloat)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_RETURN_TYPE , typeid(float).hash_code())
)
;
}
}
