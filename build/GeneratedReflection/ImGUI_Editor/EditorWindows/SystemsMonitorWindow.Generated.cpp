#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/SystemsMonitorWindow.Generated.h"
#include "ImGUI_Editor/EditorWindows/SystemsMonitorWindow.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Editor {
namespace ImGUI {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SystemsMonitorWindow>("SystemsMonitorWindow",{
"SteelEngine",
"Editor",
"ImGUI"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE, SteelEngine::Editor::SceneType::START_MENU_SCENE|SteelEngine::Editor::SceneType::EDITOR_SCENE),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SystemsMonitorWindow))
)
.Inheritance<Window>("Window")
.Constructor<>()
(
)
;
}

void SystemsMonitorWindow::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
SERIALIZE(SystemsMonitorWindow::m_CurrentItem)
SERIALIZE(SystemsMonitorWindow::m_CurrentReflectionItem)
SERIALIZE(SystemsMonitorWindow::reflectionRes)
SERIALIZE(SystemsMonitorWindow::m_Info)
SERIALIZE(SystemsMonitorWindow::m_Database)
Window::Serialize(serializer);
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
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
