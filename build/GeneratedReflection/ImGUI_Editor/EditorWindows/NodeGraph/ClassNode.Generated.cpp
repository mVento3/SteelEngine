#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/ClassNode.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/ImGUI_Editor/EditorWindows/NodeGraph/ClassNode.Generated.h"

namespace SteelEngine {
namespace NodeGraph {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<ClassNode>("ClassNode",{
"SteelEngine",
"NodeGraph"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(ClassNode))
)
.RegisterConstructor<>()
.RegisterConstructor<const ImVec2&, const ImVec2&, const std::string&>()
.RegisterInheritance<INode>("INode")
;
}
}
}
