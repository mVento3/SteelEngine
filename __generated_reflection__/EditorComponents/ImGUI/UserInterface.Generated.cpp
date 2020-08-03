#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "EditorComponents/ImGUI/UserInterface.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/EditorComponents/ImGUI/UserInterface.Generated.h"

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
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::GENERATE_OWN_SERIALIZE_FUNC, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(UserInterface))
)
.RegisterConstructor<>()
.RegisterMethod("Serialize", &UserInterface::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void UserInterface::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(UserInterface::m_Draw)
SERIALIZE(UserInterface::m_Context)
SERIALIZE(UserInterface::m_Title)
SERIALIZE(UserInterface::m_Editor)
SERIALIZE(UserInterface::m_TypeID)
SERIALIZE(UserInterface::m_IsWindowOpen)
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(UserInterface)
{
FIND_THE_RIGHT_OBJECT

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
