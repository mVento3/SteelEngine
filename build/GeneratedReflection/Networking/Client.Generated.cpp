#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Client.Generated.h"
#include "Networking/Client.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
.Inheritance<Network::INetwork>("Network::INetwork")
.Constructor<>()
.Method("Connect", &Client::Connect)
.Method("Process", &Client::Process)
.Method("Send", &Client::Send)
.Method("Receive", &Client::Receive)
.Method("GetCommands", &Client::GetCommands)
;
}

void Client::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(RuntimeDatabase::ConstructedObjectsVector* typeInfo)
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
