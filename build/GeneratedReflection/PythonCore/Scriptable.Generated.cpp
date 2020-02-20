#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "PythonCore/Scriptable.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/PythonCore/Scriptable.Generated.h"

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
.Constructor<const std::string&>()
.Method("SetPython", &Scriptable::SetPython)
.Method("GetScriptName", &Scriptable::GetScriptName)
;
}
}
}
}
