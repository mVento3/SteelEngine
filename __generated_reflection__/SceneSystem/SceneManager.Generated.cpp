#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "SceneSystem/SceneManager.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/SceneSystem/SceneManager.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<SceneManager>("SceneManager",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(SceneManager))
)
.RegisterConstructor<>()
.RegisterInheritance<ISceneManager>("ISceneManager")
;
}
}
