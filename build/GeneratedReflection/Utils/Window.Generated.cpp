#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Utils/Window.Generated.h"
#include "Utils/Window.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {
namespace Utils {
REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<Window>("Window",{
"SteelEngine",
"Utils"
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
;
}

void Window::Serialize(SteelEngine::HotReload::ISerializer* serializer)
{
SERIALIZE(Window::m_Title)
SERIALIZE(Window::m_Context)
}

}
}
