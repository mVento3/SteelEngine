#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/StartMenuWindow.Generated.h"
#include "ImGUI_Editor/EditorWindows/StartMenuWindow.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
namespace Editor {
namespace ImGUI {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<StartMenuWindow>("StartMenuWindow",{
"SteelEngine",
"Editor",
"ImGUI"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE, SteelEngine::Editor::SceneType::START_MENU_SCENE),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(SteelEngine::EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(StartMenuWindow))
)
.Inheritance<SteelEngine::EditorComponents::ImGUI::UserInterface>("SteelEngine::EditorComponents::ImGUI::UserInterface")
.Constructor<>()
.Method("Cast_UserInterface", &StartMenuWindow::Cast_UserInterface)
;
}

void StartMenuWindow::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(StartMenuWindow::m_SelectedProject)
SERIALIZE(StartMenuWindow::m_VirtualProject)
SERIALIZE(StartMenuWindow::m_ProjectsPath)
SERIALIZE(StartMenuWindow::m_LoadProjectThread)
SERIALIZE(StartMenuWindow::m_SelectableProjects)
SERIALIZE(StartMenuWindow::m_NewProjectName)
SERIALIZE(StartMenuWindow::m_Size)
UserInterface::Serialize(serializer);
UserInterface::Serialize(serializer);
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
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
