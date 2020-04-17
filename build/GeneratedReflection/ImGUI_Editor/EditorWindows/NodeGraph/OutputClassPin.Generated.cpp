#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/OutputClassPin.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/ImGUI_Editor/EditorWindows/NodeGraph/OutputClassPin.Generated.h"

namespace SteelEngine {
namespace NodeGraph {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<OutputClassPin>("OutputClassPin",{
"SteelEngine",
"NodeGraph"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(OutputClassPin))
)
.RegisterConstructor<>()
.RegisterConstructor<VisualScript::IPin::PinType, VisualScript::IPin::VariableType>()
.RegisterInheritance<VisualScript::IPin>("VisualScript::IPin")
.RegisterInheritance<VisualScript::IPinClassFunction>("VisualScript::IPinClassFunction")
.RegisterInheritance<JsonSerialization>("JsonSerialization")
.RegisterMethod("Cast_IPin", &OutputClassPin::Cast_IPin)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_IPinClassFunction", &OutputClassPin::Cast_IPinClassFunction)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_JsonSerialization", &OutputClassPin::Cast_JsonSerialization)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
;
}
}
}
