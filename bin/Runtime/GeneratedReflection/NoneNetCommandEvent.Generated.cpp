#include "NoneNetCommandEvent.Generated.h"
#include "../include\Networking\NoneNetCommandEvent.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::NetworkCommands::NoneNetCommandEvent>("NoneNetCommandEvent")
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_NETWORK_COMMAND, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::NetworkCommands::NoneNetCommandEvent))
)
.Constructor<>()
(
)
.Method("Serialize", &SteelEngine::NetworkCommands::NoneNetCommandEvent::Serialize)
(
)
.Method("Deserialize", &SteelEngine::NetworkCommands::NoneNetCommandEvent::Deserialize)
(
)
.Method("Serialize", &SteelEngine::NetworkCommands::NoneNetCommandEvent::Serialize)
(
)
.Method("Deserialize", &SteelEngine::NetworkCommands::NoneNetCommandEvent::Deserialize)
(
)
.Method("ServerSide", &SteelEngine::NetworkCommands::NoneNetCommandEvent::ServerSide)
(
)
.Method("ClientSide", &SteelEngine::NetworkCommands::NoneNetCommandEvent::ClientSide)
(
)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::NetworkCommands::NoneNetCommandEvent)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
