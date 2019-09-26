#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Networking/Commands/SynchronizeProjectNCE.Generated.h"
#include "Networking/Commands/SynchronizeProjectNCE.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Network {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SynchronizeProjectNCE>("SynchronizeProjectNCE",{
"SteelEngine",
"Network"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NETWORK_COMMAND, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SynchronizeProjectNCE))
)
.Inheritance<INetworkCommand>("INetworkCommand")
.Inheritance<Script::Python::Scriptable>("Script::Python::Scriptable")
.Constructor<>()
(
)
.Method("GetShouldOverrideEvent", &SynchronizeProjectNCE::GetShouldOverrideEvent)
(
)
.Method("Cast_INetworkCommand", &SynchronizeProjectNCE::Cast_INetworkCommand)
(
)
.Method("Cast_Scriptable", &SynchronizeProjectNCE::Cast_Scriptable)
(
)
;
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(SynchronizeProjectNCE)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
}
