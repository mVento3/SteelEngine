#include "SomeReflectionModule.Generated.h"
#include "../include\Core\SomeReflectionModule.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::SomeReflectionModule>("SomeReflectionModule")
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_REFLECTION_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::SomeReflectionModule))
)
.Constructor<>()
(
)
;
}

void SteelEngine::SomeReflectionModule::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::SomeReflectionModule)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
