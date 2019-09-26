#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/TestWindow.Generated.h"
#include "ImGUI_Editor/EditorWindows/TestWindow.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Editor {
namespace ImGUI {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<TestWindow>("TestWindow",{
"SteelEngine",
"Editor",
"ImGUI"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE, SteelEngine::Editor::SceneType::EDITOR_SCENE),
SteelEngine::Reflection::MetaData("sizeof", sizeof(TestWindow))
)
.Inheritance<Window>("Window")
.Constructor<>()
(
)
.Property("test", &TestWindow::test)
(
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::RANGE, SteelEngine::Editor::Range(0,0.5f))
)
.Property("test2", &TestWindow::test2)
(
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::RANGE, SteelEngine::Editor::Range(0,2))
)
;
}

void TestWindow::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
SERIALIZE(TestWindow::test)
SERIALIZE(TestWindow::test2)
Window::Serialize(serializer);
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(void* typeInfo)
{
DECLARE_TYPE_INFO(TestWindow)
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
