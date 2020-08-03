#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Editor/ImGUI/EditorWindows/NodeGraph/OutputPin.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Editor/ImGUI/EditorWindows/NodeGraph/OutputPin.Generated.h"

namespace SteelEngine {
namespace NodeGraph {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<OutputPin>("OutputPin",{
"SteelEngine",
"NodeGraph"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(OutputPin))
)
.RegisterConstructor<>()
.RegisterConstructor<VisualScript::IPin::PinType, VisualScript::IPin::VariableType>()
.RegisterInheritance<VisualScript::IPin>("VisualScript::IPin")
.RegisterInheritance<VisualScript::IPinFunction>("VisualScript::IPinFunction")
.RegisterInheritance<JsonSerialization>("JsonSerialization")
.RegisterMethod("Cast_IPin", &OutputPin::Cast_IPin)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_IPinFunction", &OutputPin::Cast_IPinFunction)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_JsonSerialization", &OutputPin::Cast_JsonSerialization)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
;
}
}
}
