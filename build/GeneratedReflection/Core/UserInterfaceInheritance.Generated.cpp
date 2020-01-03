#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/UserInterfaceInheritance.Generated.h"
#include "Core/UserInterfaceInheritance.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<UserInterfaceInheritance>("UserInterfaceInheritance",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::INHERITANCE_MODULE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(UserInterfaceInheritance))
)
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.Constructor<>()
.Method("ProcessInheritance", &UserInterfaceInheritance::ProcessInheritance)
;
}

void UserInterfaceInheritance::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(UserInterfaceInheritance)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
