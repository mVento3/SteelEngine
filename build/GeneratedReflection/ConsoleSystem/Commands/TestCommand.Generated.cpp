#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ConsoleSystem/Commands/TestCommand.Generated.h"
#include "ConsoleSystem/Commands/TestCommand.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<TestCommand>("TestCommand",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CONSOLE_COMMAND, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(TestCommand))
)
.Inheritance<ICommand>("ICommand")
.Constructor<>()
;
}

void TestCommand::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(TestCommand)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
