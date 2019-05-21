#include "FileSystem.Generated.h"
#include "../include\FileSystem\FileSystem.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::FileSystem>("FileSystem")
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::FileSystem))
)
.Constructor<>()
(
)
;
}

void SteelEngine::FileSystem::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::FileSystem)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
