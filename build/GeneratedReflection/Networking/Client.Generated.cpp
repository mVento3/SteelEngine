#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Client.Generated.h"
#include "Networking/Client.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<Client>("Client",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Client))
)
.Inheritance<Network::INetwork>("Network::INetwork")
.Constructor<>()
(
)
.Method("Connect", &Client::Connect)
(
)
.Method("Process", &Client::Process)
(
)
.Method("Send", &Client::Send)
(
)
.Method("Receive", &Client::Receive)
(
)
.Method("GetCommands", &Client::GetCommands)
(
)
;
}

void Client::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(Client)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
