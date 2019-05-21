#include "RuntimeCompiler.Generated.h"
#include "../include\RuntimeCompiler\RuntimeCompiler.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::RuntimeCompiler>("RuntimeCompiler")
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::RuntimeCompiler))
)
.Constructor<>()
(
)
;
}

void SteelEngine::RuntimeCompiler::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::RuntimeCompiler)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
