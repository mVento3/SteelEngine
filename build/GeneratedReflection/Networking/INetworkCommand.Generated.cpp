#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Networking/INetworkCommand.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Networking/INetworkCommand.Generated.h"

namespace SteelEngine {
namespace Network {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<INetworkCommand>("INetworkCommand",{
"SteelEngine",
"Network"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(INetworkCommand))
)
;
}
}
}
