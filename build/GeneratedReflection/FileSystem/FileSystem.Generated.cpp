#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/FileSystem/FileSystem.Generated.h"
#include "FileSystem/FileSystem.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<FileSystem>("FileSystem",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(FileSystem))
)
.Inheritance<IFileSystem>("IFileSystem")
.Constructor<>()
(
)
;
}

void FileSystem::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(FileSystem)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
