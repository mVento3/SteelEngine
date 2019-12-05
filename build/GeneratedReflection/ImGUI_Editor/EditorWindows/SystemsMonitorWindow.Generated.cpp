#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/SystemsMonitorWindow.Generated.h"
#include "ImGUI_Editor/EditorWindows/SystemsMonitorWindow.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
namespace Editor {
namespace ImGUI {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<SystemsMonitorWindow>("SystemsMonitorWindow",{
"SteelEngine",
"Editor",
"ImGUI"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE, SteelEngine::Editor::SceneType::START_MENU_SCENE|SteelEngine::Editor::SceneType::EDITOR_SCENE),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(SteelEngine::EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SystemsMonitorWindow))
)
.Inheritance<EditorComponents::ImGUI::UserInterface>("EditorComponents::ImGUI::UserInterface")
.Constructor<>()
.Method("Cast_UserInterface", &SystemsMonitorWindow::Cast_UserInterface)
;
}

void SystemsMonitorWindow::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(SystemsMonitorWindow::m_CurrentItem)
SERIALIZE(SystemsMonitorWindow::m_CurrentReflectionItem)
SERIALIZE(SystemsMonitorWindow::reflectionRes)
SERIALIZE(SystemsMonitorWindow::m_Info)
SERIALIZE(SystemsMonitorWindow::m_Database)
UserInterface::Serialize(serializer);
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(SystemsMonitorWindow)
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
