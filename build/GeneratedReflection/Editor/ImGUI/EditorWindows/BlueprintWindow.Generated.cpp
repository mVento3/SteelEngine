#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Editor/ImGUI/EditorWindows/BlueprintWindow.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/Editor/ImGUI/EditorWindows/BlueprintWindow.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<BlueprintWindow>("BlueprintWindow",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Editor::ReflectionAttributes::WINDOW_TYPE , Editor::WindowType::CONTEXT),
SteelEngine::Reflection::MetaData(Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(BlueprintWindow))
)
.RegisterConstructor<>()
.RegisterInheritance<EditorComponents::ImGUI::UserInterface>("EditorComponents::ImGUI::UserInterface")
.RegisterInheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.RegisterInheritance<EventObserver>("EventObserver")
.RegisterMethod("Cast_UserInterface", &BlueprintWindow::Cast_UserInterface)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_IRuntimeObject", &BlueprintWindow::Cast_IRuntimeObject)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_EventObserver", &BlueprintWindow::Cast_EventObserver)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Serialize", &BlueprintWindow::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void BlueprintWindow::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(BlueprintWindow::m_View)
SERIALIZE(BlueprintWindow::m_SelectedNodeID)
SERIALIZE(BlueprintWindow::m_HoveredNodeID)
SERIALIZE(BlueprintWindow::m_SelectedInputPin)
SERIALIZE(BlueprintWindow::m_SelectedOutputPin)
SERIALIZE(BlueprintWindow::m_IsAnyPinHovered)
SERIALIZE(BlueprintWindow::m_HoveredConnection)
SERIALIZE(BlueprintWindow::m_SelectedConnection)
SERIALIZE(BlueprintWindow::m_AvailableIDs)
SERIALIZE(BlueprintWindow::m_Nodes)
SERIALIZE(BlueprintWindow::m_Connections)
SERIALIZE(BlueprintWindow::m_AvailableTemplates)
SERIALIZE(BlueprintWindow::m_Keys)
SERIALIZE(BlueprintWindow::m_SavingChanged)
SERIALIZE(BlueprintWindow::m_Path)
SERIALIZE(BlueprintWindow::m_Type)
SERIALIZE(BlueprintWindow::m_Name)
SERIALIZE(BlueprintWindow::m_Object)
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
