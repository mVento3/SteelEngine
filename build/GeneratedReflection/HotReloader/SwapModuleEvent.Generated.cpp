#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/HotReloader/SwapModuleEvent.Generated.h"
#include "HotReloader/SwapModuleEvent.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SwapModuleEvent>("SwapModuleEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SwapModuleEvent))
)
.Property("m_ModuleName", &SwapModuleEvent::m_ModuleName)
;
}

}
