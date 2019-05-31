#include "RCS_ReflectionModule.Generated.h"
#include "../include\Core\RCS_ReflectionModule.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::RCS_ReflectionModule>("RCS_ReflectionModule")
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_REFLECTION_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::RCS_ReflectionModule))
)
.Inheritance("Interface::IRuntimeObject")
.Constructor<>()
(
)
;
}

void SteelEngine::RCS_ReflectionModule::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::RCS_ReflectionModule)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
