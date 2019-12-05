#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/ImGUI_Editor.Generated.h"
#include "ImGUI_Editor/ImGUI_Editor.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::EDITOR, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::EDITOR_NAME, "ImGui"),
SteelEngine::Reflection::MetaData("sizeof", sizeof(ImGUI_Editor))
)
.Inheritance<IEditor>("IEditor")
.Constructor<>()
;
}

void ImGUI_Editor::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(ImGUI_Editor::m_Context)
SERIALIZE(ImGUI_Editor::m_CurrentScene)
SERIALIZE(ImGUI_Editor::m_API_Context)
SERIALIZE(ImGUI_Editor::m_MainEditorWindows)
SERIALIZE(ImGUI_Editor::m_StartMenuWindows)
SERIALIZE(ImGUI_Editor::m_UIs)
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(RuntimeDatabase::ConstructedObjectsVector* typeInfo)
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
