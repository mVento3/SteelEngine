#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ConsoleSystem/ConsoleSystem.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ConsoleSystem/ConsoleSystem.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<ConsoleSystem>("ConsoleSystem",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(ConsoleSystem))
)
.Constructor<>()
.Inheritance<IConsoleSystem>("IConsoleSystem")
;
}
void ConsoleSystem::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(ConsoleSystem)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
