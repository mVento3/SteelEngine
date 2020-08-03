#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Editor/ImGUI/EditorWindows/NodeGraph/InputPin.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Editor/ImGUI/EditorWindows/NodeGraph/InputPin.Generated.h"

namespace SteelEngine {
namespace NodeGraph {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<InputPin>("InputPin",{
"SteelEngine",
"NodeGraph"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(InputPin))
)
.RegisterConstructor<>()
.RegisterConstructor<VisualScript::IPin::PinType, VisualScript::IPin::VariableType>()
.RegisterInheritance<VisualScript::IPin>("VisualScript::IPin")
.RegisterInheritance<VisualScript::IPinData>("VisualScript::IPinData")
.RegisterInheritance<JsonSerialization>("JsonSerialization")
.RegisterMethod("Cast_IPin", &InputPin::Cast_IPin)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_IPinData", &InputPin::Cast_IPinData)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_JsonSerialization", &InputPin::Cast_JsonSerialization)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
;
}
}
}
