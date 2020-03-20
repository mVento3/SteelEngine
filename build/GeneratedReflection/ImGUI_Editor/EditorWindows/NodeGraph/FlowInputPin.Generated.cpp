#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/FlowInputPin.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/NodeGraph/FlowInputPin.Generated.h"

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
.Constructor<>()
.Inheritance<VisualScript::IPin>("VisualScript::IPin")
.Inheritance<VisualScript::IPinFunction>("VisualScript::IPinFunction")
.Method("Cast_IPin", &FlowInputPin::Cast_IPin)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Cast_IPinFunction", &FlowInputPin::Cast_IPinFunction)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
;
}
}
}
