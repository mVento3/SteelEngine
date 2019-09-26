#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/NetworkManagerWindow.Generated.h"
#include "ImGUI_Editor/EditorWindows/NetworkManagerWindow.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Editor {
namespace ImGUI {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<NetworkManagerWindow>("NetworkManagerWindow",{
"SteelEngine",
"Editor",
"ImGUI"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE, SteelEngine::Editor::SceneType::EDITOR_SCENE|SteelEngine::Editor::SceneType::START_MENU_SCENE),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(NetworkManagerWindow))
)
.Inheritance<Window>("Window")
.Inheritance<Script::Python::Scriptable>("Script::Python::Scriptable")
.Constructor<>()
(
)
.Method("Cast_Window", &NetworkManagerWindow::Cast_Window)
(
)
.Method("Cast_Scriptable", &NetworkManagerWindow::Cast_Scriptable)
(
)
;
}

void NetworkManagerWindow::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
Window::Serialize(serializer);
Scriptable::Serialize(serializer);
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
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
