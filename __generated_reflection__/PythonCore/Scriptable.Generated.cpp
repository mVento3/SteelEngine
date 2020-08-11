#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "PythonCore/Scriptable.h"
#include "PythonCore/Scriptable.Generated.h"

namespace SteelEngine {
namespace Script {
namespace Python {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Scriptable>("Scriptable",{
"SteelEngine",
"Script",
"Python"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Scriptable))
)
.RegisterConstructor<const std::string&>()
.RegisterMethod("SetPython", { SteelEngine::Reflection::FucntionArgument<IScript*>("python") }, &Scriptable::SetPython)
.RegisterMethod("GetScriptName", &Scriptable::GetScriptName)
;
}
}
}
}
