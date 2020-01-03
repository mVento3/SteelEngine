#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/WindowInheritance.Generated.h"
#include "Core/WindowInheritance.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<WindowInheritance>("WindowInheritance",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::INHERITANCE_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(WindowInheritance))
)
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.Constructor<>()
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
