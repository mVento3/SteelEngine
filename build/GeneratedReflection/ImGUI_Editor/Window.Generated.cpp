#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/Window.Generated.h"
#include "ImGUI_Editor/Window.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
SteelEngine::Reflection::MetaData("sizeof", sizeof(Window))
)
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.Property("m_Title", &Window::m_Title)
.Property("m_Context", &Window::m_Context)
.Property("m_Editor", &Window::m_Editor)
;
}

void Window::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(Window::m_Title)
SERIALIZE(Window::m_Context)
SERIALIZE(Window::m_Editor)
}

}
}
