#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/EditorComponents/ImGUI/UserInterface.Generated.h"
#include "EditorComponents/ImGUI/UserInterface.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(UserInterface))
)
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.Constructor<>()
.Property("m_Context", &UserInterface::m_Context)
.Property("m_Title", &UserInterface::m_Title)
.Property("m_Editor", &UserInterface::m_Editor)
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
}
