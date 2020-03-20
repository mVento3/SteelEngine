#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Networking/Commands/SwapModuleNetCommandEvent.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Commands/SwapModuleNetCommandEvent.Generated.h"

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
.Constructor<>()
.Constructor<const char*>()
.Inheritance<INetworkCommand>("INetworkCommand")
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.Property("m_ModuleName", &SwapModuleNetCommandEvent::m_ModuleName)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NET_VALUE, true)
)
.Method("ServerSide", { SteelEngine::Reflection::FucntionArgument<Network::INetwork*>("network"),
SteelEngine::Reflection::FucntionArgument<SOCKET>("sock") }, &SwapModuleNetCommandEvent::ServerSide)
.Method("ClientSide", { SteelEngine::Reflection::FucntionArgument<Network::INetwork*>("network"),
SteelEngine::Reflection::FucntionArgument<SOCKET>("sock") }, &SwapModuleNetCommandEvent::ClientSide)
.Method("Cast_INetworkCommand", &SwapModuleNetCommandEvent::Cast_INetworkCommand)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Cast_IRuntimeObject", &SwapModuleNetCommandEvent::Cast_IRuntimeObject)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
;
}
}
}
