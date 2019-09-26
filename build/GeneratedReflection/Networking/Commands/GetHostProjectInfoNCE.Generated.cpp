#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Commands/GetHostProjectInfoNCE.Generated.h"
#include "Networking/Commands/GetHostProjectInfoNCE.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Network {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<GetHostProjectInfoNCE>("GetHostProjectInfoNCE",{
"SteelEngine",
"Network"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NETWORK_COMMAND, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(GetHostProjectInfoNCE))
)
.Inheritance<INetworkCommand>("INetworkCommand")
.Inheritance<Script::Python::Scriptable>("Script::Python::Scriptable")
.Constructor<>()
(
)
.Property("m_Files", &GetHostProjectInfoNCE::m_Files)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NET_VALUE, true)
)
.Method("ServerSide", &GetHostProjectInfoNCE::ServerSide)
(
)
.Method("ClientSide", &GetHostProjectInfoNCE::ClientSide)
(
)
.Method("SetLoadedProjectPath", &GetHostProjectInfoNCE::SetLoadedProjectPath)
(
)
.Method("GetFiles", &GetHostProjectInfoNCE::GetFiles)
(
)
.Method("Cast_INetworkCommand", &GetHostProjectInfoNCE::Cast_INetworkCommand)
(
)
.Method("Cast_Scriptable", &GetHostProjectInfoNCE::Cast_Scriptable)
(
)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(GetHostProjectInfoNCE)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
}
