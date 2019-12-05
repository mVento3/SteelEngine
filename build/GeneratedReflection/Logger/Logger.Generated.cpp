#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Logger/Logger.Generated.h"
#include "Logger/Logger.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Logger>("Logger",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Logger))
)
.Inheritance<ILogger>("ILogger")
.Constructor<const char*>()
.Method("Init", &Logger::Init)
.Method<void, const std::string&, int, Type::uint32, const std::string&, va_list>("Log", &Logger::Log)
.Method<void, const std::string&, int, va_list>("Log", &Logger::Log)
;
}

void Logger::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(RuntimeDatabase::ConstructedObjectsVector* typeInfo)
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
