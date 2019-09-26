#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Server.Generated.h"
#include "Networking/Server.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<Server>("Server",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::SERVER_INFO, SteelEngine::ServerInfo(1024,5004)),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Server))
)
.Inheritance<Network::INetwork>("Network::INetwork")
.Constructor<>()
(
)
.Method("Start", &Server::Start)
(
)
.Method("Send", &Server::Send)
(
)
.Method("Receive", &Server::Receive)
(
)
.Method("GetCommands", &Server::GetCommands)
(
)
;
}

void Server::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(Server)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
