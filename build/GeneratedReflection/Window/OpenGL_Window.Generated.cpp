#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Window/OpenGL_Window.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/Window/OpenGL_Window.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<OpenGL_Window>("OpenGL_Window",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(OpenGL_Window))
)
.Constructor<>()
.Inheritance<IWindow>("IWindow")
.Method("SetProcessEventsCallback", { SteelEngine::Reflection::FucntionArgument<std::function<void(void*,IWindow*)>>("callback") }, &OpenGL_Window::SetProcessEventsCallback)
.Method("GetContext", &OpenGL_Window::GetContext)
.Method("MakeCurrent", &OpenGL_Window::MakeCurrent)
;
}
}
