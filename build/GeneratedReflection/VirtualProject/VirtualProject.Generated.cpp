#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/VirtualProject/VirtualProject.Generated.h"
#include "VirtualProject/VirtualProject.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<VirtualProject>("VirtualProject",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(VirtualProject))
)
.Inheritance<IVirtualProject>("IVirtualProject")
.Inheritance<Script::Python::Scriptable>("Script::Python::Scriptable")
.Constructor<>()
.Method("Cast_IVirtualProject", &VirtualProject::Cast_IVirtualProject)
.Method("Cast_Scriptable", &VirtualProject::Cast_Scriptable)
;
}

void VirtualProject::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(VirtualProject::m_LoadedProject)
SERIALIZE(VirtualProject::m_ProjectName)
SERIALIZE(VirtualProject::m_ReflectionGenerator)
SERIALIZE(VirtualProject::m_Process)
SERIALIZE(VirtualProject::m_Compiler)
SERIALIZE(VirtualProject::m_ProjectDLL)
SERIALIZE(VirtualProject::m_ProjectConfig)
SERIALIZE(VirtualProject::m_CompileConfig)
SERIALIZE(VirtualProject::m_ProjectScripts)
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(RuntimeDatabase::ConstructedObjectsVector* typeInfo)
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
