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
void Server::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
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
