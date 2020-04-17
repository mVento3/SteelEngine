#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "VisualScript/Pin.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/VisualScript/Pin.Generated.h"

namespace SteelEngine {
namespace VisualScript {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Pin>("Pin",{
"SteelEngine",
"VisualScript"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Pin))
)
.RegisterConstructor<Pin::Flow, Pin::PinType, Pin::VariableType>()
.RegisterInheritance<IPin>("IPin")
;
}
}
}
