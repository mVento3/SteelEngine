#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/PythonCore/Scriptable.Generated.h"
#include "PythonCore/Scriptable.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

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
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Scriptable))
)
.Method("SetPython", &Scriptable::SetPython)
.Method("GetScriptName", &Scriptable::GetScriptName)
;
}

}
}
}
