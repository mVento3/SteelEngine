#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/ConsoleWindow.Generated.h"
#include "ImGUI_Editor/EditorWindows/ConsoleWindow.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<ConsoleWindow>("ConsoleWindow",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE, SteelEngine::Editor::SceneType::EDITOR_SCENE|SteelEngine::Editor::SceneType::START_MENU_SCENE),
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(ConsoleWindow))
)
.Inheritance<Editor::Window>("Editor::Window")
.Constructor<>()
(
)
;
}

void ConsoleWindow::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
Window::Serialize(serializer);
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(ConsoleWindow)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
