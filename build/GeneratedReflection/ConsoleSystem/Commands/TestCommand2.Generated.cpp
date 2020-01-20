#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ConsoleSystem/Commands/TestCommand2.Generated.h"
#include "ConsoleSystem/Commands/TestCommand2.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<TestCommand2>("TestCommand2",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::CONSOLE_COMMAND, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(TestCommand2))
)
.Inheritance<ICommand>("ICommand")
.Constructor<>()
;
}

void TestCommand2::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(TestCommand2)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
