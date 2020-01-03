#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/NetworkManagerWindow.Generated.h"
#include "ImGUI_Editor/EditorWindows/NetworkManagerWindow.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
namespace Editor {
namespace ImGUI {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<NetworkManagerWindow>("NetworkManagerWindow",{
"SteelEngine",
"Editor",
"ImGUI"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE, SteelEngine::Editor::SceneType::EDITOR_SCENE|SteelEngine::Editor::SceneType::START_MENU_SCENE),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(NetworkManagerWindow))
)
.Inheritance<EditorComponents::ImGUI::UserInterface>("EditorComponents::ImGUI::UserInterface")
.Inheritance<Script::Python::Scriptable>("Script::Python::Scriptable")
.Constructor<>()
.Method("Cast_UserInterface", &NetworkManagerWindow::Cast_UserInterface)
.Method("Cast_Scriptable", &NetworkManagerWindow::Cast_Scriptable)
;
}

void NetworkManagerWindow::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(NetworkManagerWindow)
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
