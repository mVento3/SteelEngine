#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/EditorWindows/AssetBrowserWindow.h"
#include "D:\Projects\C++\SteelEngine\build/GeneratedReflection/ImGUI_Editor/EditorWindows/AssetBrowserWindow.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<AssetBrowserWindow>("AssetBrowserWindow",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Editor::ReflectionAttributes::WINDOW_TYPE , Editor::WindowType::STATIC),
SteelEngine::Reflection::MetaData(Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(AssetBrowserWindow))
)
.RegisterConstructor<>()
.RegisterInheritance<EditorComponents::ImGUI::UserInterface>("EditorComponents::ImGUI::UserInterface")
.RegisterInheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.RegisterMethod("Cast_UserInterface", &AssetBrowserWindow::Cast_UserInterface)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_IRuntimeObject", &AssetBrowserWindow::Cast_IRuntimeObject)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Serialize", &AssetBrowserWindow::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void AssetBrowserWindow::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(AssetBrowserWindow::m_WorkingPath)
SERIALIZE(AssetBrowserWindow::m_FileIcon)
SERIALIZE(AssetBrowserWindow::m_DirectoryIcon)
SERIALIZE(AssetBrowserWindow::m_CppIcon)
SERIALIZE(AssetBrowserWindow::m_SelectedAsset)
SERIALIZE(AssetBrowserWindow::m_HoveredAsset)
SERIALIZE(AssetBrowserWindow::m_NewName)
SERIALIZE(AssetBrowserWindow::m_Items)
SERIALIZE(AssetBrowserWindow::m_FileWatcher)
SERIALIZE(AssetBrowserWindow::m_Thread)
SERIALIZE(AssetBrowserWindow::m_SelectedType)
SERIALIZE(AssetBrowserWindow::m_Types)
SERIALIZE(AssetBrowserWindow::m_TypeNames)
UserInterface::Serialize(serializer);
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(AssetBrowserWindow)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
