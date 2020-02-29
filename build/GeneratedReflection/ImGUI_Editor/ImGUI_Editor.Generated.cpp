#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/ImGUI_Editor.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/ImGUI_Editor.Generated.h"

namespace SteelEngine {
namespace Editor {
namespace ImGUI {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<ImGUI_Editor>("ImGUI_Editor",{
"SteelEngine",
"Editor",
"ImGUI"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::EDITOR, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::EDITOR_NAME , "ImGui"),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(ImGUI_Editor))
)
.Constructor<>()
.Inheritance<IEditor>("IEditor")
.Inheritance<EventObserver>("EventObserver")
.Method("Cast_IEditor", &ImGUI_Editor::Cast_IEditor)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Cast_EventObserver", &ImGUI_Editor::Cast_EventObserver)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Serialize", &ImGUI_Editor::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void ImGUI_Editor::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(ImGUI_Editor::m_Context)
SERIALIZE(ImGUI_Editor::m_CurrentScene)
SERIALIZE(ImGUI_Editor::m_API_Context)
SERIALIZE(ImGUI_Editor::m_VirtualProjectVisualizer)
SERIALIZE(ImGUI_Editor::m_NaiveManager)
SERIALIZE(ImGUI_Editor::m_MainEditorWindows)
SERIALIZE(ImGUI_Editor::m_StartMenuWindows)
SERIALIZE(ImGUI_Editor::m_UIs)
SERIALIZE(ImGUI_Editor::texture)
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(ImGUI_Editor)
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
