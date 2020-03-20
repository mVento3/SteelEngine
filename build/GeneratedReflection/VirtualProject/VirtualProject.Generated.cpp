#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "VirtualProject/VirtualProject.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/VirtualProject/VirtualProject.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<VirtualProject>("VirtualProject",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(VirtualProject))
)
.Constructor<>()
.Inheritance<IVirtualProject>("IVirtualProject")
.Inheritance<Script::Python::Scriptable>("Script::Python::Scriptable")
.Method("Cast_IVirtualProject", &VirtualProject::Cast_IVirtualProject)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Cast_Scriptable", &VirtualProject::Cast_Scriptable)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Serialize", &VirtualProject::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void VirtualProject::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(VirtualProject::m_EnginePath)
SERIALIZE(VirtualProject::m_LoadedProject)
SERIALIZE(VirtualProject::m_ProjectName)
SERIALIZE(VirtualProject::m_ReflectionGenerator)
SERIALIZE(VirtualProject::m_Process)
SERIALIZE(VirtualProject::m_Compiler)
SERIALIZE(VirtualProject::m_ProjectDLL)
SERIALIZE(VirtualProject::m_Visualizer)
SERIALIZE(VirtualProject::m_ErrorWhileCompilingOBJs)
SERIALIZE(VirtualProject::m_ProjectConfig)
SERIALIZE(VirtualProject::m_CompileConfig)
SERIALIZE(VirtualProject::m_ProjectScripts)
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
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
