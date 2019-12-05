#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/Core.Generated.h"
#include "Core/Core.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Core>("Core",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Core))
)
.Inheritance<ICore>("ICore")
.Constructor<>()
.Method("Start", &Core::Start)
.Method("Stop", &Core::Stop)
.Method("SetPathVariant", &Core::SetPathVariant)
;
}

void Core::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(Core::m_RuntimeReloader)
SERIALIZE(Core::m_ReflectionGenerator)
SERIALIZE(Core::m_Logger)
SERIALIZE(Core::m_Renderer)
SERIALIZE(Core::m_Window)
SERIALIZE(Core::m_Editor)
SERIALIZE(Core::m_VirtualProject)
SERIALIZE(Core::m_NetworkManager)
SERIALIZE(Core::m_Python)
SERIALIZE(Core::m_ImGUI_ContextAPI)
SERIALIZE(Core::m_DeltaTimeVariant)
SERIALIZE(Core::m_CompileConfig)
SERIALIZE(Core::m_Running)
SERIALIZE(Core::m_EnginePathVariant)
SERIALIZE(Core::m_ReflectionModules)
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(Core)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
