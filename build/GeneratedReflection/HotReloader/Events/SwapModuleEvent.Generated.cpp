#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "HotReloader/Events/SwapModuleEvent.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/HotReloader/Events/SwapModuleEvent.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<SwapModuleEvent>("SwapModuleEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SwapModuleEvent))
)
;
}
}
