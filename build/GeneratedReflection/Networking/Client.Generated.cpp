#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Networking/Client.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Client.Generated.h"

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
.Constructor<>()
.Inheritance<Network::INetwork>("Network::INetwork")
.Method("Connect", &Client::Connect)
.Method("Process", &Client::Process)
.Method("Send", &Client::Send)
.Method("Receive", &Client::Receive)
.Method("GetCommands", &Client::GetCommands)
;
}
}
