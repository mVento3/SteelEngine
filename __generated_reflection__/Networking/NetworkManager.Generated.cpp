#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Networking/NetworkManager.h"
#include "Networking/NetworkManager.Generated.h"

namespace SteelEngine {
namespace Network {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<NetworkManager>("NetworkManager",{
"SteelEngine",
"Network"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(NetworkManager))
)
.RegisterConstructor<>()
.RegisterInheritance<INetworkManager>("INetworkManager")
.RegisterMethod("GetConnectedClientsCount", &NetworkManager::GetConnectedClientsCount)
.RegisterMethod("GetConnectionStatus", &NetworkManager::GetConnectionStatus)
;
}
}
}
