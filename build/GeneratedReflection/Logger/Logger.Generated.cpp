#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Logger/Logger.Generated.h"
#include "Logger/Logger.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<Logger>("Logger",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Logger))
)
.Inheritance<Interface::ILogger>("Interface::ILogger")
.Constructor<const char*>()
(
)
.Method("Init", &Logger::Init)
(
)
.Method("Log", &Logger::Log)
(
)
;
}

void Logger::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(Logger)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(const char*)
};

return result;
}
#endif
}
