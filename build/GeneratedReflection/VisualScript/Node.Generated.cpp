#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "VisualScript/Node.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/VisualScript/Node.Generated.h"

namespace SteelEngine {
namespace VisualScript {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Node>("Node",{
"SteelEngine",
"VisualScript"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Node))
)
.RegisterConstructor<const std::string&>()
.RegisterInheritance<INode>("INode")
;
}
}
}
