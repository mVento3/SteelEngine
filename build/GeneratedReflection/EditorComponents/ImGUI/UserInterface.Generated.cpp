#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "EditorComponents/ImGUI/UserInterface.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/EditorComponents/ImGUI/UserInterface.Generated.h"

namespace SteelEngine {
namespace EditorComponents {
namespace ImGUI {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<UserInterface>("UserInterface",{
"SteelEngine",
"EditorComponents",
"ImGUI"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(UserInterface))
)
.Constructor<>()
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
;
}
void UserInterface::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(UserInterface::m_Context)
SERIALIZE(UserInterface::m_Title)
SERIALIZE(UserInterface::m_Editor)
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(UserInterface)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
}
namespace Editor {
namespace ImGUI {
}
}
}
