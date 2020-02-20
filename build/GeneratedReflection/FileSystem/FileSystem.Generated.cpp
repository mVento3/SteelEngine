#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "FileSystem/FileSystem.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/FileSystem/FileSystem.Generated.h"

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
.Constructor<>()
.Inheritance<IFileSystem>("IFileSystem")
;
}
}
