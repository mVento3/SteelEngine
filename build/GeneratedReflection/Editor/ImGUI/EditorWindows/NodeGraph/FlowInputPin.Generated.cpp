#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Editor/ImGUI/EditorWindows/NodeGraph/FlowInputPin.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Editor/ImGUI/EditorWindows/NodeGraph/FlowInputPin.Generated.h"

namespace SteelEngine {
namespace NodeGraph {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<FlowInputPin>("FlowInputPin",{
"SteelEngine",
"NodeGraph"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(FlowInputPin))
)
.RegisterConstructor<>()
.RegisterInheritance<VisualScript::IPin>("VisualScript::IPin")
.RegisterInheritance<VisualScript::IPinFunction>("VisualScript::IPinFunction")
.RegisterInheritance<JsonSerialization>("JsonSerialization")
.RegisterMethod("Cast_IPin", &FlowInputPin::Cast_IPin)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_IPinFunction", &FlowInputPin::Cast_IPinFunction)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_JsonSerialization", &FlowInputPin::Cast_JsonSerialization)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
;
}
}
}
