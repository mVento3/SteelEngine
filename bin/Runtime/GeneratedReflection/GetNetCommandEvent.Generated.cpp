#include "GetNetCommandEvent.Generated.h"
#include "../include\Networking\GetNetCommandEvent.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::NetworkCommands::GetNetCommandEvent>("GetNetCommandEvent")
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_NETWORK_COMMAND, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::NetworkCommands::GetNetCommandEvent))
)
.Constructor<>()
(
)
.Method("Serialize", &SteelEngine::NetworkCommands::GetNetCommandEvent::Serialize)
(
)
.Method("Deserialize", &SteelEngine::NetworkCommands::GetNetCommandEvent::Deserialize)
(
)
.Method("ServerSide", &SteelEngine::NetworkCommands::GetNetCommandEvent::ServerSide)
(
)
.Method("ClientSide", &SteelEngine::NetworkCommands::GetNetCommandEvent::ClientSide)
(
)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::NetworkCommands::GetNetCommandEvent)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
