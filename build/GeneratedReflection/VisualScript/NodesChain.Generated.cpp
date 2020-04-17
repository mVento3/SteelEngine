#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "VisualScript/NodesChain.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/VisualScript/NodesChain.Generated.h"

namespace SteelEngine {
namespace VisualScript {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<NodesChain>("NodesChain",{
"SteelEngine",
"VisualScript"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(NodesChain))
)
.RegisterConstructor<>()
;
}
}
}
