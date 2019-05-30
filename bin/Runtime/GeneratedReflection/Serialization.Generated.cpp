#include "Serialization.Generated.h"
#include "../include\Serialization\Serialization.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::Serialization>("Serialization")
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::Serialization))
)
.Constructor<>()
(
)
;
}

void SteelEngine::Serialization::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::Serialization)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
