#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Networking/Commands/SwapModuleNetCommandEvent.h"
#include "Networking/Commands/SwapModuleNetCommandEvent.Generated.h"

namespace SteelEngine {
namespace Network {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<SwapModuleNetCommandEvent>("SwapModuleNetCommandEvent",{
"SteelEngine",
"Network"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NETWORK_COMMAND, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SwapModuleNetCommandEvent))
)
.RegisterConstructor<>()
.RegisterConstructor<const char*>()
.RegisterInheritance<INetworkCommand>("INetworkCommand")
.RegisterInheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.RegisterProperty("m_ModuleName", &SwapModuleNetCommandEvent::m_ModuleName)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NET_VALUE, true)
)
.RegisterMethod("ServerSide", { SteelEngine::Reflection::FucntionArgument<Network::INetwork*>("network"),
SteelEngine::Reflection::FucntionArgument<SOCKET>("sock") }, &SwapModuleNetCommandEvent::ServerSide)
.RegisterMethod("ClientSide", { SteelEngine::Reflection::FucntionArgument<Network::INetwork*>("network"),
SteelEngine::Reflection::FucntionArgument<SOCKET>("sock") }, &SwapModuleNetCommandEvent::ClientSide)
.RegisterMethod("Cast_INetworkCommand", &SwapModuleNetCommandEvent::Cast_INetworkCommand)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_IRuntimeObject", &SwapModuleNetCommandEvent::Cast_IRuntimeObject)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
;
}
}
}
