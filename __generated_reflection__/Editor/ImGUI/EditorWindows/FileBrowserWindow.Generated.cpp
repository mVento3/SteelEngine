#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Editor/ImGUI/EditorWindows/FileBrowserWindow.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Editor/ImGUI/EditorWindows/FileBrowserWindow.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<FileBrowserWindow>("FileBrowserWindow",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Editor::ReflectionAttributes::WINDOW_TYPE , SteelEngine::Editor::WindowType::STATIC),
SteelEngine::Reflection::MetaData(Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(FileBrowserWindow))
)
.RegisterConstructor<>()
.RegisterInheritance<EditorComponents::ImGUI::UserInterface>("EditorComponents::ImGUI::UserInterface")
.RegisterInheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.RegisterMethod("Cast_UserInterface", &FileBrowserWindow::Cast_UserInterface)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_IRuntimeObject", &FileBrowserWindow::Cast_IRuntimeObject)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Serialize", &FileBrowserWindow::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void FileBrowserWindow::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(FileBrowserWindow::m_WorkingPath)
UserInterface::Serialize(serializer);
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(FileBrowserWindow)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
