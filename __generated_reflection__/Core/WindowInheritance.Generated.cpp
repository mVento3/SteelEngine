#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Core/WindowInheritance.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Core/WindowInheritance.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<WindowInheritance>("WindowInheritance",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::INHERITANCE_MODULE, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(WindowInheritance))
)
.RegisterConstructor<>()
.RegisterInheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.RegisterMethod("ProcessInheritance", { SteelEngine::Reflection::FucntionArgument<const std::vector<IReflectionInheritance*>&>("data"),
SteelEngine::Reflection::FucntionArgument<IReflectionData*>("type"),
SteelEngine::Reflection::FucntionArgument<HotReloader::IRuntimeObject*>("object") }, &WindowInheritance::ProcessInheritance)
.RegisterMethod("Serialize", &WindowInheritance::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void WindowInheritance::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(WindowInheritance)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
