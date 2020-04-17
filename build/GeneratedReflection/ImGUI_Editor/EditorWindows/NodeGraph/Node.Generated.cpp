#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/Node.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/ImGUI_Editor/EditorWindows/NodeGraph/Node.Generated.h"

namespace SteelEngine {
namespace NodeGraph {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Node>("Node",{
"SteelEngine",
"NodeGraph"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Node))
)
.RegisterConstructor<>()
.RegisterConstructor<const ImVec2&, const ImVec2&, const std::string&>()
.RegisterInheritance<INode>("INode")
;
}
}
}
