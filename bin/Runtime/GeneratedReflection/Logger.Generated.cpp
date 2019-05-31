#include "Logger.Generated.h"
#include "../include\Logger\Logger.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::Logger>("Logger")
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::Logger))
)
.Inheritance("Interface::ILogger")
.Constructor<const char*>()
(
)
.Method("Init", &SteelEngine::Logger::Init)
(
)
.Method("Log", &SteelEngine::Logger::Log)
(
)
;
}

void SteelEngine::Logger::Serialize(SteelEngine::Interface::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)
{
DECLARE_TYPE_INFO(SteelEngine::Logger)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(const char*)
};

return result;
}
#endif
