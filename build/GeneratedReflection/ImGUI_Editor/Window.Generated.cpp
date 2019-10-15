#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/Window.Generated.h"
#include "ImGUI_Editor/Window.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Editor {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<Window>("Window",{
"SteelEngine",
"Editor"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Window))
)
.Inheritance<HotReload::IRuntimeObject>("HotReload::IRuntimeObject")
.Property("m_Title", &Window::m_Title)
(
)
.Property("m_Context", &Window::m_Context)
(
)
.Property("m_Editor", &Window::m_Editor)
(
)
;
}

void Window::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
SERIALIZE(Window::m_Title)
SERIALIZE(Window::m_Context)
SERIALIZE(Window::m_Editor)
IRuntimeObject::Serialize(serializer);
}

}
}
