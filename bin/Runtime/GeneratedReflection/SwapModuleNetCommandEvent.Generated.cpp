#include "SwapModuleNetCommandEvent.Generated.h"
#include "../include\Networking\SwapModuleNetCommandEvent.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::NetworkCommands::SwapModuleNetCommandEvent>("SwapModuleNetCommandEvent")
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_NETWORK_COMMAND, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::NetworkCommands::SwapModuleNetCommandEvent))
)
.Constructor<>()
(
)
.Constructor<const std::string&>()
(
)
.Property("m_ModuleName", &SteelEngine::NetworkCommands::SwapModuleNetCommandEvent::m_ModuleName)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_NET_VALUE, true)
)
.Method("ServerSide", &SteelEngine::NetworkCommands::SwapModuleNetCommandEvent::ServerSide)
(
)
.Method("ClientSide", &SteelEngine::NetworkCommands::SwapModuleNetCommandEvent::ClientSide)
(
)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::NetworkCommands::SwapModuleNetCommandEvent)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
COMPARE_CONSTRUCTOR(const std::string&)
};

return result;
}
#endif
