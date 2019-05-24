#include "INetworkCommand.Generated.h"
#include "../include\Networking\INetworkCommand.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::NetworkCommands::INetworkCommand>("INetworkCommand")
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::NetworkCommands::INetworkCommand))
)
.Method("Serialize", &SteelEngine::NetworkCommands::INetworkCommand::Serialize)
(
)
.Method("Deserialize", &SteelEngine::NetworkCommands::INetworkCommand::Deserialize)
(
)
;
}

