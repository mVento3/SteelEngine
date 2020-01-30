#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Projects/C++/SteelEngine/include/VirtualProject/VirtualProject.Generated.h"
#include "Projects/C++/SteelEngine/include/VirtualProject/VirtualProject.h"
#include "HotReloader/RuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::VirtualProject>("VirtualProject")
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::VirtualProject))
)
.Inheritance("IVirtualProject")
.Constructor<>()
(
)
;
}

void SteelEngine::VirtualProject::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(SteelEngine::VirtualProject::m_LoadedProject)
SERIALIZE(SteelEngine::VirtualProject::m_ReflectionGenerator)
SERIALIZE(SteelEngine::VirtualProject::m_Process)
SERIALIZE(SteelEngine::VirtualProject::m_ProjectDLL)
SERIALIZE(SteelEngine::VirtualProject::m_ProjectName)
SERIALIZE(SteelEngine::VirtualProject::m_ProjectScripts)
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::VirtualProject)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
