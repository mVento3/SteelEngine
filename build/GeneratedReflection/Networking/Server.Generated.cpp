#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Networking/Server.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Server.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Server>("Server",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::SERVER_INFO , SteelEngine::ServerInfo(1024, 5004)),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Server))
)
.Constructor<>()
.Inheritance<Network::INetwork>("Network::INetwork")
.Method("Start", &Server::Start)
.Method("Send", { SteelEngine::Reflection::FucntionArgument<SOCKET>("sock"),
SteelEngine::Reflection::FucntionArgument<const char*>("buffer"),
SteelEngine::Reflection::FucntionArgument<Type::uint32>("size") }, &Server::Send)
.Method("Receive", { SteelEngine::Reflection::FucntionArgument<SOCKET>("sock"),
SteelEngine::Reflection::FucntionArgument<char*>("buffer"),
SteelEngine::Reflection::FucntionArgument<Type::uint32>("size") }, &Server::Receive)
.Method("GetCommands", { SteelEngine::Reflection::FucntionArgument<SOCKET>("sock") }, &Server::GetCommands)
;
}
}
