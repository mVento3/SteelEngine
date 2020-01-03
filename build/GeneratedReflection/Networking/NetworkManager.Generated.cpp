#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/NetworkManager.Generated.h"
#include "Networking/NetworkManager.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
.Inheritance<INetworkManager>("INetworkManager")
.Constructor<>()
.Method("GetConnectedClientsCount", &NetworkManager::GetConnectedClientsCount)
.Method("GetConnectionStatus", &NetworkManager::GetConnectionStatus)
;
}

void NetworkManager::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
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
