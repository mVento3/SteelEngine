#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Networking/Client.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Networking/Client.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Client>("Client",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Client))
)
.RegisterConstructor<>()
.RegisterInheritance<Network::INetwork>("Network::INetwork")
.RegisterMethod("Connect", { SteelEngine::Reflection::FucntionArgument<const char*>("ip") }, &Client::Connect)
.RegisterMethod("Subprocess", &Client::Subprocess)
.RegisterMethod("Send", { SteelEngine::Reflection::FucntionArgument<SOCKET>("sock"),
SteelEngine::Reflection::FucntionArgument<const char*>("buffer"),
SteelEngine::Reflection::FucntionArgument<Type::uint32>("size") }, &Client::Send)
.RegisterMethod("Receive", { SteelEngine::Reflection::FucntionArgument<SOCKET>("sock"),
SteelEngine::Reflection::FucntionArgument<char*>("buffer"),
SteelEngine::Reflection::FucntionArgument<Type::uint32>("size") }, &Client::Receive)
.RegisterMethod("GetCommands", &Client::GetCommands)
;
}
}
