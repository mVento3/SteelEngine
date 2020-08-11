#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Window/OpenGL_Window.h"
#include "Window/OpenGL_Window.Generated.h"

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
.RegisterConstructor<>()
.RegisterInheritance<IWindow>("IWindow")
.RegisterMethod("GetContext", &OpenGL_Window::GetContext)
.RegisterMethod("MakeCurrent", &OpenGL_Window::MakeCurrent)
;
}
}
