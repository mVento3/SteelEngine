#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/FileSystem/FileSystem.Generated.h"
#include "FileSystem/FileSystem.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<FileSystem>("FileSystem",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(FileSystem))
)
.Inheritance<IFileSystem>("IFileSystem")
.Constructor<>()
;
}

void FileSystem::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
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
