#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Core/PythonScriptableInheritance.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/Core/PythonScriptableInheritance.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<PythonScriptableInheritance>("PythonScriptableInheritance",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::INHERITANCE_MODULE, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(PythonScriptableInheritance))
)
.Constructor<>()
.Method("ProcessInheritance", { SteelEngine::Reflection::FucntionArgument<const std::vector<IReflectionInheritance*>&>("data"),
SteelEngine::Reflection::FucntionArgument<IReflectionData*>("type"),
SteelEngine::Reflection::FucntionArgument<void*>("object") }, &PythonScriptableInheritance::ProcessInheritance)
;
}
}
