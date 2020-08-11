#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "SceneSystem/SceneManager.h"
#include "SceneSystem/SceneManager.Generated.h"

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
