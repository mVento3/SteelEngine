#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/EditorWindows/NodeGraph/InputPin.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/NodeGraph/InputPin.Generated.h"

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
.Constructor<VisualScript::IPin::PinType, VisualScript::IPin::VariableType>()
.Inheritance<VisualScript::IPin>("VisualScript::IPin")
.Inheritance<VisualScript::IPinData>("VisualScript::IPinData")
.Method("Cast_IPin", &InputPin::Cast_IPin)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Cast_IPinData", &InputPin::Cast_IPinData)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
;
}
}
}
