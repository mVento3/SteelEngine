#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Editor/ImGUI/EditorWindows/StartMenuWindow.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Editor/ImGUI/EditorWindows/StartMenuWindow.Generated.h"

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
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::WINDOW_TYPE , SteelEngine::Editor::WindowType::STATIC),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(SteelEngine::EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(StartMenuWindow))
)
.RegisterConstructor<>()
.RegisterInheritance<EditorComponents::ImGUI::UserInterface>("EditorComponents::ImGUI::UserInterface")
.RegisterInheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.RegisterMethod("Cast_UserInterface", &StartMenuWindow::Cast_UserInterface)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_IRuntimeObject", &StartMenuWindow::Cast_IRuntimeObject)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Serialize", &StartMenuWindow::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void StartMenuWindow::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(StartMenuWindow::m_SelectedProject)
SERIALIZE(StartMenuWindow::m_VirtualProject)
SERIALIZE(StartMenuWindow::m_ProjectsPath)
SERIALIZE(StartMenuWindow::m_LoadProjectThread)
SERIALIZE(StartMenuWindow::m_ProjectInitialized)
SERIALIZE(StartMenuWindow::m_SelectableProjects)
SERIALIZE(StartMenuWindow::m_NewProjectName)
SERIALIZE(StartMenuWindow::m_Size)
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
