#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/RuntimeCompiler/SwapModuleEvent.Generated.h"
#include "RuntimeCompiler/SwapModuleEvent.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SwapModuleEvent>("SwapModuleEvent",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(SwapModuleEvent))
)
.Property("m_ModuleName", &SwapModuleEvent::m_ModuleName)
(
)
;
}

void SwapModuleEvent::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
}

}
