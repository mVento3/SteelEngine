#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "VisualScript/Nodes/AddNode.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/VisualScript/Nodes/AddNode.Generated.h"

namespace SteelEngine {
namespace VisualScript {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<AddNode>("AddNode",{
"SteelEngine",
"VisualScript"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_NODE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(AddNode))
)
.Constructor<>()
.Inheritance<Node>("Node")
.Method("SomeFunction", { SteelEngine::Reflection::FucntionArgument<int>("a"),
SteelEngine::Reflection::FucntionArgument<int>("b") }, &AddNode::SomeFunction)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::VISUAL_SCRIPT_FUNC, true)
)
;
}
}
}
