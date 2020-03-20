#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/Nodes/AddFloat.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/NodeGraph/Nodes/AddFloat.Generated.h"

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
.Constructor<>()
.Inheritance<VisualScript::NodeTemplate>("VisualScript::NodeTemplate")
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
