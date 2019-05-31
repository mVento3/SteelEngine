#include "Server.Generated.h"
#include "../include\Core\Server.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::Server>("Server")
(
SteelEngine::Reflection::MetaData(SteelEngine::SE_SERVER_INFO, SteelEngine::ServerInfo(1024,54000)),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::Server))
)
.Inheritance("Interface::INetwork")
.Constructor<>()
(
)
.Method("Start", &SteelEngine::Server::Start)
(
)
.Method("Send", &SteelEngine::Server::Send)
(
)
.Method("Receive", &SteelEngine::Server::Receive)
(
)
.Method("GetCommands", &SteelEngine::Server::GetCommands)
(
)
;
}

void SteelEngine::Server::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::Server)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
