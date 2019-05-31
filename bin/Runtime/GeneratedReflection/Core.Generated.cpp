#include "Core.Generated.h"
#include "../include\Core\Core.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::Core>("Core")
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::Core))
)
.Inheritance("Interface::ICore")
.Constructor<>()
(
)
.Property("ta", &SteelEngine::Core::ta)
(
SteelEngine::Reflection::MetaData("lol", 1212),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_RUNTIME_SERIALIZE, 11),
SteelEngine::Reflection::MetaData(SteelEngine::Core::Lol::TES, 22)
)
.Method("Start", &SteelEngine::Core::Start)
(
)
.Method("Stop", &SteelEngine::Core::Stop)
(
)
.Method("SetPathVariant", &SteelEngine::Core::SetPathVariant)
(
)
;
}

void SteelEngine::Core::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
SERIALIZE(SteelEngine::Core::m_RuntimeCompiler)
SERIALIZE(SteelEngine::Core::m_Network)
SERIALIZE(SteelEngine::Core::m_Logger)
SERIALIZE(SteelEngine::Core::m_Running)
SERIALIZE(SteelEngine::Core::m_EnginePathVariant)
SERIALIZE(SteelEngine::Core::meta)
SERIALIZE(SteelEngine::Core::meta2)
SERIALIZE(SteelEngine::Core::meta3)
SERIALIZE(SteelEngine::Core::ta)
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::Core)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
