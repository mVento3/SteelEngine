#include "FileWatcher.Generated.h"
#include "../include\FileWatcher\FileWatcher.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::FileWatcher>("FileWatcher")
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::FileWatcher))
)
.Constructor<const std::string&, const std::string&, const std::string&>()
(
)
.Property("m_Path", &SteelEngine::FileWatcher::m_Path)
(
)
;
}

void SteelEngine::FileWatcher::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::FileWatcher)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(const std::string&, const std::string&, const std::string&)
};

return result;
}
#endif
