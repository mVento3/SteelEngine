#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Editor/ImGUI/EditorWindows/DeferredBuffersViewWindow.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/Editor/ImGUI/EditorWindows/DeferredBuffersViewWindow.Generated.h"

namespace SteelEngine {
namespace Editor {
namespace ImGUI {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<DeferredBuffersViewWindow>("DeferredBuffersViewWindow",{
"SteelEngine",
"Editor",
"ImGUI"
}
)
(
SteelEngine::Reflection::MetaData(Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData(Editor::ReflectionAttributes::WINDOW_TYPE , SteelEngine::Editor::WindowType::STATIC),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(DeferredBuffersViewWindow))
)
.RegisterConstructor<>()
.RegisterInheritance<EditorComponents::ImGUI::UserInterface>("EditorComponents::ImGUI::UserInterface")
.RegisterInheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.RegisterMethod("Cast_UserInterface", &DeferredBuffersViewWindow::Cast_UserInterface)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Cast_IRuntimeObject", &DeferredBuffersViewWindow::Cast_IRuntimeObject)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.RegisterMethod("Serialize", &DeferredBuffersViewWindow::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void DeferredBuffersViewWindow::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(DeferredBuffersViewWindow::m_Textures)
UserInterface::Serialize(serializer);
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(DeferredBuffersViewWindow)
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
