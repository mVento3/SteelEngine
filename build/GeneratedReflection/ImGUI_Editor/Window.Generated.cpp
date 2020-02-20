#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/Window.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/Window.Generated.h"

namespace SteelEngine {
namespace Editor {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Window>("Window",{
"SteelEngine",
"Editor"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Window))
)
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.Method("Serialize", &Window::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void Window::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(Window::m_Title)
SERIALIZE(Window::m_Context)
SERIALIZE(Window::m_Editor)
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(Window)
{
FIND_THE_RIGHT_OBJECT

};

return result;
}
#endif
namespace ImGUI {
}
}
}
