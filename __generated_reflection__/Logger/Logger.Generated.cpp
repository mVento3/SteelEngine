#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Logger/Logger.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Logger/Logger.Generated.h"

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
.RegisterConstructor<const char*>()
.RegisterInheritance<ILogger>("ILogger")
.RegisterMethod("Init", &Logger::Init)
.RegisterMethod<void, const std::string&, int, Type::uint32, const std::filesystem::path&, va_list>("Log", { SteelEngine::Reflection::FucntionArgument<const std::string&>("message"),
SteelEngine::Reflection::FucntionArgument<int>("verbosity"),
SteelEngine::Reflection::FucntionArgument<Type::uint32>("line"),
SteelEngine::Reflection::FucntionArgument<const std::filesystem::path&>("file"),
SteelEngine::Reflection::FucntionArgument<va_list>("args") }, &Logger::Log)
.RegisterMethod<void, const std::string&, int, va_list>("Log", { SteelEngine::Reflection::FucntionArgument<const std::string&>("message"),
SteelEngine::Reflection::FucntionArgument<int>("verbosity"),
SteelEngine::Reflection::FucntionArgument<va_list>("args") }, &Logger::Log)
;
}
}
