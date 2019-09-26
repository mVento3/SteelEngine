#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/INetworkCommand.Generated.h"
#include "Networking/INetworkCommand.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Network {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<INetworkCommand>("INetworkCommand",{
"SteelEngine",
"Network"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(INetworkCommand))
)
.Inheritance<HotReload::IRuntimeObject>("HotReload::IRuntimeObject")
.Property("m_Flow", &INetworkCommand::m_Flow)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NET_VALUE, true)
)
.Property("m_Header", &INetworkCommand::m_Header)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NET_VALUE, true)
)
.Property("m_Deserialized", &INetworkCommand::m_Deserialized)
(
)
.Property("m_Busy", &INetworkCommand::m_Busy)
(
)
;
}

}
}
