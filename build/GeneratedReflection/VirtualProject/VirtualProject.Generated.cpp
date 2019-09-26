#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/VirtualProject/VirtualProject.Generated.h"
#include "VirtualProject/VirtualProject.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<VirtualProject>("VirtualProject",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(VirtualProject))
)
.Inheritance<IVirtualProject>("IVirtualProject")
.Constructor<>()
(
)
;
}

void VirtualProject::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
SERIALIZE(VirtualProject::m_LoadedProject)
SERIALIZE(VirtualProject::m_ReflectionGenerator)
SERIALIZE(VirtualProject::m_Process)
SERIALIZE(VirtualProject::m_ProjectDLL)
SERIALIZE(VirtualProject::m_ProjectName)
SERIALIZE(VirtualProject::m_ProjectConfig)
SERIALIZE(VirtualProject::m_CompileConfig)
SERIALIZE(VirtualProject::m_ObjectFiles)
SERIALIZE(VirtualProject::m_ProjectScripts)
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(VirtualProject)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
