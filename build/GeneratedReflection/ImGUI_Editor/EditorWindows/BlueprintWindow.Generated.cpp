#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/EditorWindows/BlueprintWindow.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/BlueprintWindow.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<BlueprintWindow>("BlueprintWindow",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Editor::ReflectionAttributes::SCENE_TYPE , SteelEngine::Editor::SceneType::EDITOR_SCENE | SteelEngine::Editor::SceneType::START_MENU_SCENE),
SteelEngine::Reflection::MetaData(Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(BlueprintWindow))
)
.Constructor<>()
.Inheritance<EditorComponents::ImGUI::UserInterface>("EditorComponents::ImGUI::UserInterface")
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.Inheritance<EventObserver>("EventObserver")
.Method("Cast_UserInterface", &BlueprintWindow::Cast_UserInterface)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Cast_IRuntimeObject", &BlueprintWindow::Cast_IRuntimeObject)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Cast_EventObserver", &BlueprintWindow::Cast_EventObserver)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Serialize", &BlueprintWindow::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void BlueprintWindow::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(BlueprintWindow::m_Nodes)
SERIALIZE(BlueprintWindow::m_View)
SERIALIZE(BlueprintWindow::m_SelectedNodeID)
SERIALIZE(BlueprintWindow::m_HoveredNodeID)
SERIALIZE(BlueprintWindow::m_SelectedInputPin)
SERIALIZE(BlueprintWindow::m_SelectedOutputPin)
SERIALIZE(BlueprintWindow::m_IsAnyPinHovered)
SERIALIZE(BlueprintWindow::m_Connections)
SERIALIZE(BlueprintWindow::m_HoveredConnection)
SERIALIZE(BlueprintWindow::m_SelectedConnection)
SERIALIZE(BlueprintWindow::m_AvailableIDs)
SERIALIZE(BlueprintWindow::m_AvailableTemplates)
SERIALIZE(BlueprintWindow::m_AvailableMethods)
SERIALIZE(BlueprintWindow::m_Nodes2)
SERIALIZE(BlueprintWindow::m_Keys)
SERIALIZE(BlueprintWindow::m_SavingChanged)
SERIALIZE(BlueprintWindow::m_Type)
UserInterface::Serialize(serializer);
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(BlueprintWindow)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
