#include "Client.Generated.h"
#include "../include\Networking\Client.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::Client>("Client")
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::Client))
)
.Constructor<>()
(
)
.Method("Connect", &SteelEngine::Client::Connect)
(
)
.Method("Process", &SteelEngine::Client::Process)
(
)
.Method("Send", &SteelEngine::Client::Send)
(
)
.Method("Receive", &SteelEngine::Client::Receive)
(
)
;
}

void SteelEngine::Client::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
SERIALIZE(SteelEngine::Client::m_Socket)
SERIALIZE(SteelEngine::Client::m_Thread)
SERIALIZE(SteelEngine::Client::m_PendingCommands)
SERIALIZE(SteelEngine::Client::m_ServerInfo)
SERIALIZE(SteelEngine::Client::m_SendingHugeBlock)
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::Client)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
