#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Commands/SwapModuleNetCommandEvent.Generated.h"
#include "Networking/Commands/SwapModuleNetCommandEvent.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Network {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SwapModuleNetCommandEvent>("SwapModuleNetCommandEvent",{
"SteelEngine",
"Network"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NETWORK_COMMAND, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SwapModuleNetCommandEvent))
)
.Inheritance<INetworkCommand>("INetworkCommand")
.Constructor<>()
(
)
.Constructor<const char*>()
(
)
.Property("m_ModuleName", &SwapModuleNetCommandEvent::m_ModuleName)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NET_VALUE, true)
)
.Property("m_Buffer", &SwapModuleNetCommandEvent::m_Buffer)
(
)
.Method("ServerSide", &SwapModuleNetCommandEvent::ServerSide)
(
)
.Method("ClientSide", &SwapModuleNetCommandEvent::ClientSide)
(
)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(SwapModuleNetCommandEvent)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
COMPARE_CONSTRUCTOR(const char*)
};

return result;
}
#endif
}
}
