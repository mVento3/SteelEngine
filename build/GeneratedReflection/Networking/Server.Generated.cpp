#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Networking/Server.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Server.Generated.h"

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
.Method("Send", &Server::Send)
.Method("Receive", &Server::Receive)
.Method("GetCommands", &Server::GetCommands)
;
}
}
