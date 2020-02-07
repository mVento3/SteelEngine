#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Core/WindowInheritance.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/WindowInheritance.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<WindowInheritance>("WindowInheritance",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::INHERITANCE_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(WindowInheritance))
)
.Constructor<>()
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.Method("ProcessInheritance", &WindowInheritance::ProcessInheritance)
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
