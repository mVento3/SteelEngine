#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Core/UserInterfaceInheritance.h"
#include "Core/UserInterfaceInheritance.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<UserInterfaceInheritance>("UserInterfaceInheritance",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::INHERITANCE_MODULE, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(UserInterfaceInheritance))
)
.RegisterConstructor<>()
.RegisterInheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.RegisterMethod("ProcessInheritance", { SteelEngine::Reflection::FucntionArgument<const std::vector<IReflectionInheritance*>&>("data"),
SteelEngine::Reflection::FucntionArgument<IReflectionData*>("type"),
SteelEngine::Reflection::FucntionArgument<HotReloader::IRuntimeObject*>("object") }, &UserInterfaceInheritance::ProcessInheritance)
.RegisterMethod("Serialize", &UserInterfaceInheritance::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
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
