#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Networking/Server.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Networking/Server.Generated.h"

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
.RegisterConstructor<>()
.RegisterInheritance<Network::INetwork>("Network::INetwork")
.RegisterMethod("Start", &Server::Start)
.RegisterMethod("Send", { SteelEngine::Reflection::FucntionArgument<SOCKET>("sock"),
SteelEngine::Reflection::FucntionArgument<const char*>("buffer"),
SteelEngine::Reflection::FucntionArgument<Type::uint32>("size") }, &Server::Send)
.RegisterMethod("Receive", { SteelEngine::Reflection::FucntionArgument<SOCKET>("sock"),
SteelEngine::Reflection::FucntionArgument<char*>("buffer"),
SteelEngine::Reflection::FucntionArgument<Type::uint32>("size") }, &Server::Receive)
.RegisterMethod("GetCommands", { SteelEngine::Reflection::FucntionArgument<SOCKET>("sock") }, &Server::GetCommands)
;
}
}
