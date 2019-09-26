#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/StartMenuWindow.Generated.h"
#include "ImGUI_Editor/EditorWindows/StartMenuWindow.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Editor {
namespace ImGUI {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<StartMenuWindow>("StartMenuWindow",{
"SteelEngine",
"Editor",
"ImGUI"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE, SteelEngine::Editor::SceneType::START_MENU_SCENE),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(StartMenuWindow))
)
.Inheritance<Window>("Window")
.Constructor<>()
(
)
;
}

void StartMenuWindow::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
SERIALIZE(StartMenuWindow::m_SelectedProject)
SERIALIZE(StartMenuWindow::m_VirtualProject)
SERIALIZE(StartMenuWindow::m_ProjectsPath)
SERIALIZE(StartMenuWindow::m_LoadProjectThread)
SERIALIZE(StartMenuWindow::m_SelectableProjects)
SERIALIZE(StartMenuWindow::m_NewProjectName)
Window::Serialize(serializer);
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(StartMenuWindow)
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
