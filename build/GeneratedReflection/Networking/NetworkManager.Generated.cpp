#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/NetworkManager.Generated.h"
#include "Networking/NetworkManager.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Network {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<NetworkManager>("NetworkManager",{
"SteelEngine",
"Network"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(NetworkManager))
)
.Inheritance<INetworkManager>("INetworkManager")
.Constructor<>()
(
)
.Method("GetConnectedClientsCount", &NetworkManager::GetConnectedClientsCount)
(
)
.Method("GetConnectionStatus", &NetworkManager::GetConnectionStatus)
(
)
;
}

void NetworkManager::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(NetworkManager)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
}
